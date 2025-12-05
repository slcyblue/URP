#include "URPMonsterCharacter.h"
#include "AIController.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Core/Subsystems/Data/URPGameDataSubsystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "URPMonsterAIController.h"
#include "Net/UnrealNetwork.h"
#include <Characters/Player/URPPlayerCharacter.h>

AURPMonsterCharacter::AURPMonsterCharacter()
{
    bReplicates = true;
    bIsActive = false;

    AggroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AggroSphere"));
    AggroSphere->SetupAttachment(RootComponent);

    // 기본 충돌 설정 (Overlap으로 Pawn만 감지)
    AggroSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    AggroSphere->SetCollisionObjectType(ECC_WorldDynamic);
    AggroSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    AggroSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AURPMonsterCharacter::BeginPlay()
{
    Super::BeginPlay();

    AggroSphere->SetSphereRadius(AggroRadius);

    AggroSphere->OnComponentBeginOverlap.AddDynamic(this, &AURPMonsterCharacter::OnAggroEnter);
}

void AURPMonsterCharacter::SetActive(bool bActive)
{
    bIsActive = bActive;

    if (bActive)
    {
        SetActorHiddenInGame(false);
        SetActorEnableCollision(true);
        SetActorTickEnabled(true);

        GetCharacterMovement()->SetMovementMode(MOVE_Walking);

        // Controller 없으면 생성 (UnPossess 안 쓰기 때문에 자동 Possess 안됨)
        if (!GetController())
        {
            SpawnDefaultController();
        }

        // 다음 프레임에서 BT 시작 (Controller가 붙길 기다림)
        FTimerHandle TempHandle;
        GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
            {
                AURPMonsterAIController* AI = Cast<AURPMonsterAIController>(GetController());
                if (!AI) return;

                UBlackboardComponent* BB = AI->GetBlackboardComponent();
                if (BB)
                {
                    BB->SetValueAsVector("HomeLocation", GetActorLocation());
                    BB->SetValueAsEnum("AIState", (uint8)EAIState::Idle);
                    BB->SetValueAsObject("TargetActor", nullptr);
                    BB->SetValueAsBool("HasTarget", false);
                    BB->SetValueAsBool("IsInAttackRange", false);
                    BB->SetValueAsFloat("AttackCooldown", 0.f);
                }

                AI->RunBehaviorTree(AI->BehaviorTree);
                if (AI->BrainComponent)
                {
                    AI->BrainComponent->StartLogic();
                }
            });
    }
    else
    {
        ClearTarget();

        if (AURPMonsterAIController* AI = Cast<AURPMonsterAIController>(GetController()))
        {
            if (AI->BrainComponent)
            {
                AI->BrainComponent->StopLogic(TEXT("Monster Deactivated"));
            }
        }

        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);
        GetCharacterMovement()->DisableMovement();
        SetActorTickEnabled(false);
    }
}

void AURPMonsterCharacter::InitializeFromMonsterData(const FString MonsterName, int32 DifficultyLevel)
{
    UURPGameDataSubsystem* GDS = GetGameInstance()->GetSubsystem<UURPGameDataSubsystem>();
    const FURPMonsterRow* Data = GDS->GetMonsterRow(MonsterName);
    const FURPPathConfig& Path = GDS->GetPathConfig();

    if (Data)
    {
        ApplyAppearance(*Data, Path, DifficultyLevel);
        ApplyStats(*Data, DifficultyLevel);
    }

    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
}

void AURPMonsterCharacter::ApplyAppearance(const FURPMonsterRow& Data, const FURPPathConfig& Path, int32 DifficultyLevel)
{
    // SkeletalMesh
    if (!Data.MeshPath.IsEmpty())
    {
        USkeletalMesh* SkeletalMesh = LoadObject<USkeletalMesh>(nullptr, *Data.MeshPath);
        if (SkeletalMesh)
        {
            GetMesh()->SetSkeletalMesh(SkeletalMesh);
        }
    }

    // AnimBlueprint
    if (!Path.DefaultMonsterAnimBP.IsEmpty())
    {
        UClass* AnimClass = LoadClass<UAnimInstance>(nullptr, *Path.DefaultMonsterAnimBP);
        if (AnimClass)
        {
            GetMesh()->SetAnimInstanceClass(AnimClass);
        }
        if (AnimClass) GetMesh()->SetAnimInstanceClass(AnimClass);
    }

    // 머티리얼 색상 변조
    if (UMaterialInstanceDynamic* MID = GetMesh()->CreateAndSetMaterialInstanceDynamic(0))
    {
        FLinearColor Color;

        switch (DifficultyLevel)
        {
        case 1: Color = FLinearColor::Blue; break;
        case 2: Color = FLinearColor::Green; break;
        case 3: Color = FLinearColor::Yellow; break;
        case 4: Color = FLinearColor::Red; break;
        default: Color = FLinearColor::White; break;
        }

        MID->SetVectorParameterValue(TEXT("TintColor"), Color);
        MID->SetVectorParameterValue(TEXT("TintColor"), FLinearColor(1, 1, 1));
    }
}

void AURPMonsterCharacter::ApplyStats(const FURPMonsterRow& Data, int32 DifficultyLevel)
{
    float MaxHp = Data.MaxHp * FMath::Pow(1.2f, DifficultyLevel - 1);
    float AttackPower = Data.Attack * FMath::Pow(1.15f, DifficultyLevel - 1);
    StatComponent->SetBaseStats(MaxHp, AttackPower);

    CurrentHp = StatComponent->GetFinalMaxHp();
}

void AURPMonsterCharacter::SetTarget(AActor* NewTargetActor)
{
    if (!HasAuthority()) return;

    // 이미 타겟 있음 → 타겟 변경 금지
    if (CurrentTarget && CurrentTarget != NewTargetActor)
        return;

    // 이미 설정된 경우는 통과
    if (CurrentTarget == NewTargetActor)
        return;

    CurrentTarget = Cast<AURPCharacterBase>(NewTargetActor);

    // 죽음 감지 등록
    if (CurrentTarget)
        CurrentTarget->OnCharacterDied.AddDynamic(this, &AURPMonsterCharacter::OnTargetDied);

    // 블랙보드 반영
    if (auto* AI = Cast<AURPMonsterAIController>(GetController()))
        if (auto* BB = AI->GetBlackboardComponent())
            BB->SetValueAsObject("TargetActor", NewTargetActor);
}

void AURPMonsterCharacter::OnTargetDied(AURPCharacterBase* Dead)
{
    if (!HasAuthority()) return;
    if (Dead != CurrentTarget) return;

    ClearTarget();
}

void AURPMonsterCharacter::ClearTarget()
{
    if (!HasAuthority()) return;

    if (IsValid(CurrentTarget))
    {
        CurrentTarget->OnCharacterDied.RemoveDynamic(this, &AURPMonsterCharacter::OnTargetDied);
    }

    CurrentTarget = nullptr;

    if (AURPMonsterAIController* AI = Cast<AURPMonsterAIController>(GetController()))
    {
        if (UBlackboardComponent* BB = AI->GetBlackboardComponent())
        {
            BB->SetValueAsObject("TargetActor", nullptr);
        }
    }
}

void AURPMonsterCharacter::OnAggroEnter(UPrimitiveComponent*, AActor* OtherActor,
    UPrimitiveComponent*, int32, bool, const FHitResult&)
{
    if (!HasAuthority()) return;


    auto* Player = Cast<AURPPlayerCharacter>(OtherActor);
    if (!Player) return;

    // 이미 타겟 있음 → 새로운 타겟으로 바꾸지 않음
    if (CurrentTarget)
        return;

    // 현재 AIState 확인 (Idle/Patrol일 때만 타겟 획득)
    if (auto* AI = Cast<AURPMonsterAIController>(GetController()))
    {
        if (auto* BB = AI->GetBlackboardComponent())
        {
            uint8 State = BB->GetValueAsEnum("AIState");

            if (State != (uint8)EAIState::Idle &&
                State != (uint8)EAIState::Patrol)
            {
                // Chase/Attack/Return 중에는 새 타겟 획득 불가
                return;
            }
        }
    }

    // 여기까지 통과한 경우에만 타겟 획득
    SetTarget(Player);
}


void AURPMonsterCharacter::PerformBasicAttack(AActor* TargetActor)
{
    if (AURPCharacterBase* T = Cast<AURPCharacterBase>(TargetActor))
    {
        T->ApplyDamage(GetAttackPower());
    }
}

void AURPMonsterCharacter::Die()
{
    ClearTarget();
    bIsActive = false;

    if (AURPMonsterAIController* AI = Cast<AURPMonsterAIController>(GetController()))
    {
        if (auto* BB = AI->GetBlackboardComponent())
            BB->SetValueAsEnum("AIState", (uint8)EAIState::Dead);

        if (AI->BrainComponent)
            AI->BrainComponent->StopLogic("Dead");
    }

    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    DetachFromControllerPendingDestroy();
}

void AURPMonsterCharacter::HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    //bIsAttacking = false;

    // AI 관련 작업이 있으면 여기
}