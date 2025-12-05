#pragma once

#include "CoreMinimal.h"
#include "Characters/Common/URPCharacterBase.h"
#include "Characters/Common/URPEquipmentComponent.h"
#include "Characters/Common/URPClassComponent.h"
#include "Core/GamePlay/Mechanics/Skill/System/URPSkillComponent.h"
#include "Core/GamePlay/Mechanics/Combat/URPCombatComponent.h"
#include "Core/GamePlay/Flow/World/URPCameraManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UURPCameraManager* CameraManager;
    
    UPROPERTY(VisibleAnywhere)
    UURPCombatComponent* CombatComponent;
    
    UPROPERTY(VisibleAnywhere)
    UURPClassComponent* ClassComponent;

    UPROPERTY(VisibleAnywhere)
    UURPEquipmentComponent* EquipmentComponent;

    UPROPERTY()
    FPlayerData CachedPlayerData;

    bool bPlayerDataReady = false;
    bool bBeginPlayCompleted = false;

    void SetPlayerData(const FPlayerData& Data);

    void InitializeFromPlayerData();

    void PlayAttack();
    void PlaySkill(int32 SkillId);
    
    virtual void Die();
    virtual void HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsAttacking = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bAttackDelay = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsUsingSkill = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsMoving = false;

    FTimerHandle AttackDelayHandle;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    void OnAttackDelayEnd();
};
