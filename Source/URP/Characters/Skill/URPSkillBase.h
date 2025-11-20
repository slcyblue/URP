#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Characters/URPPlayerCharacter.h"
#include "URPSkillBase.generated.h"

UCLASS(Blueprintable, Abstract)
class URP_API UURPSkillBase : public UObject
{
    GENERATED_BODY()

public:
    virtual void Execute(class AURPPlayerCharacter* Owner) PURE_VIRTUAL(UURPSkillBase::Execute, );

    int32 SkillId;
    float Cooldown;
    float NextUseTime;

    bool IsReady(float ServerTime) const
    {
        return ServerTime >= NextUseTime;
    }

    void StartCooldown(float ServerTime)
    {
        NextUseTime = ServerTime + Cooldown;
    }

    // 공통 데미지 계산
    float GetDamage(AURPPlayerCharacter* Owner) const
    {
        return Owner ? Owner->AttackPower : 0.f;
    }
};
