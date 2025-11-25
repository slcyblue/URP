#include "URPPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
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

    Anim = Cast<UURPPlayerAnimInstance>(GetMesh()->GetAnimInstance());

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
    StatComponent->Recalculate();

    UE_LOG(LogTemp, Log, TEXT("Player initialized from PlayerData"));
}


void AURPPlayerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (Anim)
    {
        Anim->Speed = GetVelocity().Size();
        Anim->bIsMoving = Anim->Speed > 5.f;
    }
}


void AURPPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AURPPlayerCharacter::PlayAttack()
{
    if (Anim)
    {
        Anim->bIsAttacking = true;
        // 몽타주 재생 등 가능
    }
}

void AURPPlayerCharacter::PlaySkill()
{
    if (Anim)
    {
        Anim->bIsUsingSkill = true;
        // 스킬 애니메이션 재생 등
    }
}
