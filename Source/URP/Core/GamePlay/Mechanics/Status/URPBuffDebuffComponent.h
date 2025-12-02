#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/URPSkillTypes.h"
#include "URPBuffDebuffComponent.generated.h"

class AURPCharacterBase;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class URP_API UURPBuffDebuffComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UURPBuffDebuffComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick, FActorComponentTickFunction*) override;

    // ----- 적용 -----
    void ApplyBuff(EURPBuffType Type, float Value, float Duration);
    void ApplyDebuff(EURPDebuffType Type, float Value, float Duration);
    void ApplyDOT(float TickDamage, float TickInterval, float Duration);

    // ----- 원본 값 반환 (StatComponent가 최종 계산) -----
    float GetTotalBuffValue(EURPBuffType Type) const;
    float GetTotalDebuffValue(EURPDebuffType Type) const;

    float GetFlat(EURPBuffType Type) const;      // Flat형 증가량
    float GetPercent(EURPBuffType Type) const;   // Percent형 증가량 (0.2 = +20%)

    bool IsStunned() const;
    bool IsSilenced() const;

private:
    UPROPERTY()
    AURPCharacterBase* OwnerCharacter = nullptr;

    UPROPERTY()
    TArray<FURPActiveBuff> ActiveBuffs;

    UPROPERTY()
    TArray<FURPActiveDebuff> ActiveDebuffs;

    UPROPERTY()
    TArray<FURPActiveDOT> ActiveDOTs;

    static float PercentToRate(float PercentValue)
    {
        return PercentValue * 0.01f;
    }
};
