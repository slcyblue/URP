#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/URPSkillTypes.h"
#include "URPBuffDebuffComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class URP_API UURPBuffDebuffComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // ====== Apply API ======
    void ApplyBuff(EURPBuffType Type, float Value, float Duration);
    void ApplyDebuff(EURPDebuffType Type, float Value, float Duration);
    void ApplyDOT(float TickDamage, float TickInterval, float Duration);

    // ====== 쿼리 (스탯 반영용) ======
    float GetAttackMultiplier() const;      // 기본 1.0
    float GetDefenseMultiplier() const;     // 기본 1.0
    float GetMoveSpeedMultiplier() const;   // 기본 1.0
    float GetSkillHasteMultiplier() const;  // 기본 1.0 (쿨감)

    bool  IsStunned() const;
    bool  IsSilenced() const;

protected:
    UPROPERTY()
    TArray<FURPActiveBuff> ActiveBuffs;

    UPROPERTY()
    TArray<FURPActiveDebuff> ActiveDebuffs;

    UPROPERTY()
    TArray<FURPActiveDOT> ActiveDOTs;

private:
    static float PercentToRate(float PercentValue);
};
