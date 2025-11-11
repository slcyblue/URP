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
    CameraBoom->TargetArmLength = 1200.f;
    CameraBoom->SetRelativeRotation(FRotator(-55.f, 0.f, 0.f));
    CameraBoom->bUsePawnControlRotation = false;
    CameraBoom->bInheritPitch = false;
    CameraBoom->bInheritYaw = false;
    CameraBoom->bInheritRoll = false;
    CameraBoom->bDoCollisionTest = true;
    CameraBoom->ProbeSize = 8.f;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;


    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);
    bUseControllerRotationYaw = false;

    AIControllerClass = AAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AURPPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("[PlayerCharacter] %s BeginPlay"), *GetName());
}


void AURPPlayerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    UpdateCameraTransparency(DeltaSeconds);
}


void AURPPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AURPPlayerCharacter::UpdateCameraTransparency(float DeltaTime)
{
    // 충돌 여부에 따라 카메라 거리 조정
    float DesiredLength = 800.f;
    if (CameraBoom->bDoCollisionTest && CameraBoom->IsCollisionFixApplied())
    {
        DesiredLength = FMath::Lerp(CameraBoom->TargetArmLength, 400.f, DeltaTime * 5.f);
    }
    else
    {
        DesiredLength = FMath::Lerp(CameraBoom->TargetArmLength, 800.f, DeltaTime * 2.f);
    }
    CameraBoom->TargetArmLength = DesiredLength;
}
