#include "URPPlayerController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "EngineUtils.h"


AURPPlayerController::AURPPlayerController()
{
    DefaultMouseCursor = EMouseCursor::Default;

    bBlockInput = false;
    bAutoManageActiveCameraTarget = true;
}

void AURPPlayerController::BeginPlay()
{
    Super::BeginPlay();

    CachedCharacter = Cast<AURPPlayerCharacter>(GetPawn());
    if (!CachedCharacter)
    {
        // Pawn이 아직 스폰 안된 경우를 대비
        GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
            {
                CachedCharacter = Cast<AURPPlayerCharacter>(GetPawn());
            });
    }

    FInputModeGameAndUI InputMode; // 또는 FInputModeGameOnly
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);

    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    UE_LOG(LogTemp, Log, TEXT("[PlayerController] InputMode set & Cursor enabled"));
}

void AURPPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        UE_LOG(LogTemp, Log, TEXT("[PlayerController] InputComponent OK"));
        InputComponent->BindAction("Move", IE_Pressed, this, &AURPPlayerController::OnMovePressed);
        InputComponent->BindAction("Move", IE_Released, this, &AURPPlayerController::OnMoveReleased);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[PlayerController] InputComponent NULL!"));
    }
}

void AURPPlayerController::OnMovePressed()
{
    bIsMoving = true;
    HandleContinuousMove(); // 즉시 한 번 호출
}

void AURPPlayerController::OnMoveReleased()
{
    bIsMoving = false;
}

void AURPPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsMoving)
    {
        HandleContinuousMove();
    }
}

void AURPPlayerController::HandleContinuousMove()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_GameTraceChannel1, false, Hit); // GroundTrace 채널 사용

    if (Hit.bBlockingHit)
    {
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Hit.ImpactPoint);
    }
}

void AURPPlayerController::FindTargetUnderCursor()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Pawn, false, Hit);

    if (AActor* HitActor = Hit.GetActor())
    {
        if (HitActor != CurrentTarget)
        {
            HighlightTarget(HitActor);
            CurrentTarget = HitActor;
        }
    }
    else
    {
        HighlightTarget(nullptr);
        CurrentTarget = nullptr;
    }
}

void AURPPlayerController::HighlightTarget(AActor* Target)
{
    for (auto Actor : TActorRange<ACharacter>(GetWorld()))
    {
        if (auto* Mesh = Cast<USkeletalMeshComponent>(Actor->GetComponentByClass(USkeletalMeshComponent::StaticClass())))
        {
            Mesh->SetRenderCustomDepth(false);
        }
    }

    if (Target)
    {
        if (auto* Mesh = Cast<USkeletalMeshComponent>(Target->GetComponentByClass(USkeletalMeshComponent::StaticClass())))
        {
            Mesh->SetRenderCustomDepth(true);
            Mesh->CustomDepthStencilValue = 1; // 포스트프로세스에서 outline
        }
    }
}