#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Types/URPCommonEnums.h"
#include "Characters/Common/URPStatComponent.h"
#include "URPCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDied, AURPCharacterBase*, DeadCharacter);

class UURPBuffDebuffComponent;
class UURPSkillComponent;
class UURPAnimInstance;

UCLASS()
class URP_API AURPCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AURPCharacterBase();

protected:
	virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
    int64 CurrentHp = 100;

    UPROPERTY(VisibleAnywhere)
    UURPBuffDebuffComponent* BuffDebuffComp;

    UPROPERTY(VisibleAnywhere)
    UURPSkillComponent* SkillComponent;

    UPROPERTY(VisibleAnywhere)
    UURPStatComponent* StatComponent;

    UPROPERTY(VisibleAnywhere)
    UURPAnimInstance* AnimInstance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsDead = false;

    UPROPERTY(BlueprintAssignable)
    FOnCharacterDied OnCharacterDied;

    // 공통 행동
    virtual void ApplyDamage(float Amount);

    virtual void HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    UFUNCTION(BlueprintCallable)
    virtual void RecalculateStats();


    UFUNCTION(BlueprintCallable)
    float GetAttackPower() const { return StatComponent->GetFinalAttack(); }

    UFUNCTION(BlueprintCallable)
    float GetDefensePower() const { return StatComponent->GetFinalDefense(); }

    UFUNCTION(BlueprintCallable)
    float GetAttackSpeed() const { return StatComponent->GetFinalAttackSpeed(); }
};
