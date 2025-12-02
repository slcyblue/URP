#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "URPAnimInstance.generated.h"

class AURPCharacterBase;

/*
 *  UURPAnimInstance
 *  -----------------
 *  - 공격/스킬 애니메이션을 통합 관리
 *  - 공격속도 배율 적용
 *  - AnimNotify → 캐릭터로 콜백 전달
 *  - 몽타주 종료 후 콜백 전달 (SkillComponent/CharacterComponent)
 */
UCLASS()
class URP_API UURPAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UURPAnimInstance();

    // Character Owner
    UPROPERTY(BlueprintReadOnly, Category = "Owner")
    TWeakObjectPtr<AURPCharacterBase> OwnerCharacter;

    // 초기화
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable, Category = "Animation")
    float PlayNormalMontage(UAnimMontage* Montage);

    UFUNCTION(BlueprintCallable, Category = "Animation")
    float PlayCombatMontage(UAnimMontage* Montage, float AnimSpeed = 1.f);

    // 스톱
    UFUNCTION(BlueprintCallable, Category = "Animation")
    void StopAllMontages(float BlendOutTime = 0.25f);

    UFUNCTION()
    void IsMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Speed = 0.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsMoving = false;

private:
    float CachedAttackSpeed = 1.f;
};
