#include "URPMonsterCharacter.h"
#include "AIController.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Core/Subsystems/Data/URPGameDataSubsystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "URPMonsterAIController.h"

AURPMonsterCharacter::AURPMonsterCharacter()
{
    bReplicates = true;             // Actor 복제 활성화
    bIsActive = false;
}

void AURPMonsterCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AURPMonsterCharacter::SetActive(bool bActive)
{
    bIsActive = bActive;

    if (bActive)
    {
        // 활성화 상태
        SetActorHiddenInGame(false);
        SetActorEnableCollision(true);
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

        if (AURPMonsterAIController* AI = Cast<AURPMonsterAIController>(GetController()))
        {
            if (UBlackboardComponent* BB = AI->GetBlackboardComponent())
            {
                BB->SetValueAsVector("HomeLocation", GetActorLocation());
            }
        }
    }
    else
    {
        // 비활성 상태 (Pool로 돌아감)
        ClearTarget();
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);
        GetCharacterMovement()->DisableMovement();
    }
}

void AURPMonsterCharacter::InitializeFromMonsterData(const FString MonsterName, int32 DifficultyLevel)
{
    UURPGameDataSubsystem* GDS = GetGameInstance()->GetSubsystem<UURPGameDataSubsystem>();
    const FURPMonsterRow* Data = GDS->GetMonsterRow(MonsterName);
    const FURPPathConfig& Path = GDS->GetPathConfig();
    ApplyAppearance(*Data , Path, DifficultyLevel);
    ApplyStats(*Data, DifficultyLevel);
    ApplyAI(Path);

    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
    SetActorTickEnabled(true);
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
    }

    // 머티리얼 색상 변조
    UMaterialInstanceDynamic* MID = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
    if (MID)
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
    }
}

void AURPMonsterCharacter::ApplyStats(const FURPMonsterRow& Data, int32 DifficultyLevel)
{
    float LevelHpMultiplier = FMath::Pow(1.20f, DifficultyLevel - 1);
    float LevelAtkMultiplier = FMath::Pow(1.15f, DifficultyLevel - 1);

    MaxHp *= LevelHpMultiplier;
    AttackPower *= LevelAtkMultiplier;

    /*MoveSpeed = Data.MoveSpeed * Mult;

    GetCharacterMovement()->MaxWalkSpeed = MoveSpeed*/
}

void AURPMonsterCharacter::ApplyAI(const FURPPathConfig& Path)
{
    if (!Path.DefaultMonsterBT.IsEmpty())
    {
        UBehaviorTree* BT = LoadObject<UBehaviorTree>(nullptr, *Path.DefaultMonsterBT);
        if (BT)
        {
            if (AAIController* AIC = Cast<AAIController>(GetController()))
            {
                AIC->RunBehaviorTree(BT);
            }
        }
    }
}

void AURPMonsterCharacter::SetTargetFromBlackboard(AActor* NewTargetActor)
{
    if (!HasAuthority()) return;


    // 1) NewTarget 유효성 검사
    if (!IsValid(NewTargetActor))
    {
        ClearTarget();
        return;
    }

    AURPCharacterBase* NewTarget = Cast<AURPCharacterBase>(NewTargetActor);

    // 2) 캐스팅 실패 / 자기 자신 / 이미 죽은 대상이면 타겟 해제
    if (!IsValid(NewTarget) || NewTarget == this || NewTarget->bIsDead)
    {
        ClearTarget();
        return;
    }

    // 3) 현재 타겟과 동일하면 다시 바인딩할 필요 없음
    if (CurrentTarget == NewTarget)
        return;

    // 4) 기존 타겟 언바인딩
    if (IsValid(CurrentTarget))
    {
        CurrentTarget->OnCharacterDied.RemoveDynamic(
            this, &AURPMonsterCharacter::OnTargetDied);
    }

    // 5) 새 타겟 설정 + 구독
    CurrentTarget = NewTarget;

    CurrentTarget->OnCharacterDied.AddDynamic(
        this, &AURPMonsterCharacter::OnTargetDied);
}

void AURPMonsterCharacter::OnTargetDied(AURPCharacterBase* Dead)
{
    if (!HasAuthority()) return;
    if (Dead != CurrentTarget) return;

    CurrentTarget = nullptr;

    ClearTarget();

    // Blackboard 정리 + Return 상태로 전환
    if (AURPMonsterAIController* AI = Cast<AURPMonsterAIController>(GetController()))
    {
        UBlackboardComponent* BB = AI->GetBlackboardComponent();
        if (BB)
        {
            BB->SetValueAsObject("TargetActor", nullptr);
            BB->SetValueAsBool("HasTarget", false);
            BB->SetValueAsEnum("AIState", (uint8)EAIState::Return);
        }
    }
}

void AURPMonsterCharacter::ClearTarget()
{
    if (!HasAuthority())
        return;

    if (IsValid(CurrentTarget))
    {
        CurrentTarget->OnCharacterDied.RemoveDynamic(
            this, &AURPMonsterCharacter::OnTargetDied);
    }

    CurrentTarget = nullptr;
}

void AURPMonsterCharacter::PerformBasicAttack(AActor* TargetActor)
{
    AURPCharacterBase* Target = Cast<AURPCharacterBase>(TargetActor);
    if (!Target) return;

    float FinalDamage = AttackPower;

    // 방어력 시스템이 있다면 여기에 추가 가능
    // FinalDamage = FinalDamage * (100 / (100 + Target->DefensePower));

    UE_LOG(LogTemp, Log, TEXT("[MonsterAttack] %s → %s : %.1f damage"),
        *GetName(), *Target->GetName(), FinalDamage);

    Target->ApplyDamage(FinalDamage);
}

void AURPMonsterCharacter::Die()
{
    if (!bIsActive)
        return;

    ClearTarget();

    bIsActive = false;

    // AI, 이동 끄기
    GetCharacterMovement()->DisableMovement();
    DetachFromControllerPendingDestroy();

    // Death 애니메이션 재생
    if (DeathMontage)
    {
        UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();

        if (AnimInst)
        {
            AnimInst->Montage_Play(DeathMontage);
            FOnMontageEnded EndDelegate;
            EndDelegate.BindUObject(this, &AURPMonsterCharacter::OnDeathMontageEnded);

            AnimInst->Montage_SetEndDelegate(EndDelegate, DeathMontage);
            return;
        }
    }


    // AI Reset
    if (AAIController* AI = Cast<AAIController>(GetController()))
    {
        if (UBlackboardComponent* BB = AI->GetBlackboardComponent())
        {
            BB->ClearValue("TargetActor");
            BB->SetValueAsBool("HasTarget", false);
            BB->SetValueAsEnum("AIState", (uint8)EAIState::Dead);
        }
        AI->StopMovement();
    }

    // 몽타주 없으면 즉시 반환
    OnDeathMontageEnded(nullptr, true);
}

void AURPMonsterCharacter::OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (OwningZone && OwningZone->MonsterPool)
    {
        OwningZone->MonsterPool->ReturnMonster(this);
    }
}