#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Characters/Player/URPPlayerCharacter.h"
#include "URPPlayerController.generated.h"

UCLASS()
class URP_API AURPPlayerController : public APlayerController
{
    GENERATED_BODY()

    AURPPlayerController();

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY()
    AActor* CurrentTarget;

    UFUNCTION()
    void FindTargetUnderCursor();

    UFUNCTION()
    void HighlightTarget(AActor* Target);

    void HandleContinuousMove(); // 매 프레임 이동 갱신

    bool bIsMovingContinuous = false;

    void MoveToCursorOnce();       // 한 번 이동
    void UpdateMoveIfHolding();    // 누르고 있는 동안 이동 갱신

private:
    UPROPERTY()
    AURPPlayerCharacter* CachedCharacter;
};
