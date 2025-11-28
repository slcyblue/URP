#include "URPPlayerController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "EngineUtils.h"
#include <Core/Subsystems/UI/URPInputSubsystem.h>


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
        // Pawn이 늦게 Possess되는 경우 지원
        GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
            {
                CachedCharacter = Cast<AURPPlayerCharacter>(GetPawn());
            });
    }

    // 마우스 설정
    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);

    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;

    // Enhanced InputSubsystem 등록
    if (UURPInputSubsystem* InputSub = GetLocalPlayer()->GetSubsystem<UURPInputSubsystem>())
    {
        InputSub->SetupInput(this);   // IA & IMC 바인딩은 여기서만 처리됨
    }
}

void AURPPlayerController::MoveToCursorOnce()
{
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (Hit.bBlockingHit)
    {
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Hit.ImpactPoint);
    }
}

void AURPPlayerController::UpdateMoveIfHolding()
{
    if (!bIsMovingContinuous) return;

    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (Hit.bBlockingHit)
    {
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Hit.ImpactPoint);
    }
}

void AURPPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    UpdateMoveIfHolding();
    FindTargetUnderCursor();
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
    // 기존 아웃라인 해제
    for (auto Actor : TActorRange<ACharacter>(GetWorld()))
    {
        if (auto* Mesh = Actor->FindComponentByClass<USkeletalMeshComponent>())
        {
            Mesh->SetRenderCustomDepth(false);
        }
    }

    // 새 타겟에 아웃라인 적용
    if (Target)
    {
        if (auto* Mesh = Target->FindComponentByClass<USkeletalMeshComponent>())
        {
            Mesh->SetRenderCustomDepth(true);
            Mesh->CustomDepthStencilValue = 1;
        }
    }
}