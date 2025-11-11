#pragma once

#include "CoreMinimal.h"
#include "URPCharacterBase.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "URPPlayerCharacter.generated.h"

/**
 * 플레이어 전용 캐릭터 클래스
 * - 입력 처리
 * - 카메라 제어
 * - UI 연동
 */
UCLASS()
class URP_API AURPPlayerCharacter : public AURPCharacterBase
{
    GENERATED_BODY()

public:
    AURPPlayerCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    // 카메라 셋팅
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    void UpdateCameraTransparency(float DeltaTime);
};
