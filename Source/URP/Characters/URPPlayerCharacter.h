#pragma once

#include "CoreMinimal.h"
#include "URPCharacterBase.h"
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
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    /** 카메라 붐 (3rd Person용) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    /** 실제 카메라 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

    /** 이동 입력 */
    void MoveForward(float Value);
    void MoveRight(float Value);
};
