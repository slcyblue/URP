#include "URPPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Characters/Common/URPAnimInstance.h"
#include "Core/GamePlay/Mechanics/Skill/Base/URPSkillBase.h"
#include "AIController.h"

AURPPlayerCharacter::AURPPlayerCharacter()
{
    // === 쿼터뷰 카메라 설정 ===
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 2000.f;
    CameraBoom->SetRelativeRotation(FRotator(-55.f, 0.f, 0.f));
    CameraBoom->bUsePawnControlRotation = false;
    CameraBoom->bInheritPitch = false;
    CameraBoom->bInheritYaw = false;
    CameraBoom->bInheritRoll = false;
    CameraBoom->ProbeSize = 8.f;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;


    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
    bUseControllerRotationYaw = false;

    AIControllerClass = AAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    CameraManager = CreateDefaultSubobject<UURPCameraManager>(TEXT("CameraManager"));
    CombatComponent = CreateDefaultSubobject<UURPCombatComponent>("Combat");
    ClassComponent = CreateDefaultSubobject<UURPClassComponent>("Class");
    EquipmentComponent = CreateDefaultSubobject<UURPEquipmentComponent>("Equip");
}

void AURPPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (CameraManager)
    {
        CameraManager->InitializeCamera(CameraBoom, FollowCamera);
    }

    bBeginPlayCompleted = true;

    // 데이터가 먼저 들어왔으면 지금 초기화 진행
    if (bPlayerDataReady)
    {
        InitializeFromPlayerData();
    }

    UE_LOG(LogTemp, Log, TEXT("[PlayerCharacter] %s BeginPlay"), *GetName());
}

void AURPPlayerCharacter::SetPlayerData(const FPlayerData& Data)
{
    CachedPlayerData = Data;
    bPlayerDataReady = true;

    // BeginPlay가 이미 끝났으면 바로 초기화
    if (bBeginPlayCompleted)
    {
        InitializeFromPlayerData();
    }
}

void AURPPlayerCharacter::InitializeFromPlayerData()
{
    if (!bPlayerDataReady || !bBeginPlayCompleted)
        return;

    ClassComponent->SetClass(CachedPlayerData.SelectedClass);
    EquipmentComponent->ApplyEquipment(ClassComponent->GetClassData());
    SkillComponent->InitializeSkills(CachedPlayerData.SelectedClass);
    SkillComponent->EnsureValidSkillSlots(
        CachedPlayerData.SelectedClass,
        CachedPlayerData.SkillLevels
    );
    StatComponent->Recalculate();

    CurrentHp = StatComponent->GetFinalMaxHp();
    UE_LOG(LogTemp, Log, TEXT("Player initialized from PlayerData"));
}



void AURPPlayerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (AnimInstance)
    {
        AnimInstance-> Speed = GetVelocity().Size();
        bIsMoving = AnimInstance-> Speed > 5.f;
    }
}


void AURPPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AURPPlayerCharacter::Die()
{
    UE_LOG(LogTemp, Warning, TEXT("[Player] %s died!"), *GetName());

    // 입력 차단
    DisableInput(Cast<APlayerController>(GetController()));

    UAnimMontage* DeathMontage = ClassComponent->GetClassData()->DeathAnim;
    if (DeathMontage != nullptr)
    {
        float Duration = AnimInstance->PlayNormalMontage(DeathMontage);
        UE_LOG(LogTemp, Warning, TEXT("Montage Duration = %.2f"), Duration);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to get default montage DeathMontage"));
    }

    // 상태 업데이트
    bIsDead = true;
    OnCharacterDied.Broadcast(this);
    // 나중에 Respawn / UI 처리 추가 가능
}

void AURPPlayerCharacter::PlayAttack()
{
    if (bIsAttacking || bAttackDelay)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attack ignored: still attacking or in delay"));
        return;
    }

    UAnimMontage* AttackMontage = ClassComponent->GetClassData()->AttackAnim;
    if (!AttackMontage)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to get default montage AttackMontage"));
        return;
    }

    // 상태 업데이트
    bIsAttacking = true;
    bAttackDelay = true;

    float APS = GetAttackSpeed();
    float Cooldown = 1.f / APS;

    // 애니메이션 재생 속도 (APS 기반)
    float PlayRate = APS / ClassComponent->GetClassData()->BaseAttackSpeed;

    float Duration = AnimInstance->PlayCombatMontage(AttackMontage, PlayRate);

    // 공격 가능 시점 타이머
    GetWorldTimerManager().SetTimer(
        AttackDelayHandle,
        this,
        &AURPPlayerCharacter::OnAttackDelayEnd,
        Cooldown,
        false
    );
}

void AURPPlayerCharacter::OnAttackDelayEnd()
{
    bAttackDelay = false;
}

void AURPPlayerCharacter::PlaySkill(int32 SkillId)
{
    if (!AnimInstance || !SkillComponent)
        return;

    bIsUsingSkill = true;

    UURPSkillBase* Skill = SkillComponent->GetSkill(SkillId);
    if (!Skill)
        return;

    UAnimMontage* SkillMontage = Skill->SkillAnim;
    if (!SkillMontage)
    {
        UE_LOG(LogTemp, Warning, TEXT("Skill %d has no SkillAnim Montage"), SkillId);
        return;
    }

    float FinalASPD = GetAttackSpeed();   // 예: 1.2
    float Duration = AnimInstance->PlayCombatMontage(SkillMontage, FinalASPD);

    UE_LOG(LogTemp, Log, TEXT("[Player] Skill %d Montage Duration = %.2f"), SkillId, Duration);
}

void AURPPlayerCharacter::HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    // [1] 공격 몽타주
    if (Montage == ClassComponent->GetClassData()->AttackAnim)
    {
        bIsAttacking = false;
        return;
    }

    // [2] 사망 몽타주
    if (Montage == ClassComponent->GetClassData()->DeathAnim)
    {
        // Respawn 가능
        return;
    }

    // [3] 스킬 몽타주
    /*if (SkillComponent && SkillComponent->IsSkillMontage(Montage))
    {
        SkillComponent->OnSkillMontageEnded(Montage);
        bIsUsingSkill = false;
        return;
    }*/
}