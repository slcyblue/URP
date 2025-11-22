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
    Combat = CreateDefaultSubobject<UURPPlayerCombatComponent>("Combat");
    ClassComponent = CreateDefaultSubobject<UURPPlayerClassComponent>("Class");
    Equipment = CreateDefaultSubobject<UURPPlayerEquipmentComponent>("Equip");
    SkillComponent = CreateDefaultSubobject<UURPPlayerSkillComponent>("Skill");
}

void AURPPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (CameraManager)
    {
        CameraManager->InitializeCamera(CameraBoom, FollowCamera);
    }

    Anim = Cast<UURPPlayerAnimInstance>(GetMesh()->GetAnimInstance());
    UE_LOG(LogTemp, Log, TEXT("[PlayerCharacter] %s BeginPlay"), *GetName());
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
