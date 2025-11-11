#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "URPPlayerController.generated.h"

UCLASS()
class URP_API AURPPlayerController : public APlayerController
{
    GENERATED_BODY()

    AURPPlayerController();

public:
    virtual void BeginPlay() override;

    virtual void SetupInputComponent() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY()
    AActor* CurrentTarget;

    UFUNCTION()
    void FindTargetUnderCursor();

    UFUNCTION()
    void HighlightTarget(AActor* Target);

    UFUNCTION()
    void OnMovePressed();    // 마우스 누름 시작
    UFUNCTION()
    void OnMoveReleased();   // 마우스 뗌
    void HandleContinuousMove(); // 매 프레임 이동 갱신

private:
    bool bIsMoving = false;
};
