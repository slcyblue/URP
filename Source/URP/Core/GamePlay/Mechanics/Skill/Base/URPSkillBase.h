#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Characters/Common/URPCharacterBase.h"
#include "URPSkillBase.generated.h"

UCLASS(Blueprintable, Abstract)
class URP_API UURPSkillBase : public UObject
{
    GENERATED_BODY()

public:
    int32 SkillId = 0;
    float Cooldown = 0.f;
    float NextUseTime = 0.f;
    int32 ApplyDotSkillId = 0;

    UPROPERTY()
    UAnimMontage* SkillAnim = nullptr;

    UPROPERTY()
    AURPCharacterBase* OwnerPC;

    virtual void Execute(class AURPCharacterBase* Owner) PURE_VIRTUAL(UURPSkillBase::Execute, );

    virtual bool IsReady(float ServerTime) const
    {
        return ServerTime >= NextUseTime;
    }

    virtual void StartCooldown(float ServerTime)
    {
        NextUseTime = ServerTime + Cooldown;
    }

    virtual float GetDamage(AURPCharacterBase* Owner) const
    {
        return Owner->GetAttackPower();
    }
};
