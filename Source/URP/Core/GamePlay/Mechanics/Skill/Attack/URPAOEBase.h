#pragma once
#include "CoreMinimal.h"
#include "Core/GamePlay/Mechanics/Skill/Base/URPSkillBase.h"
#include "URPAOEBase.generated.h"

UCLASS()
class URP_API UURPAOEBase : public UURPSkillBase
{
    GENERATED_BODY()

public:
    float Radius = 0.f;
    float DamageMultiplier = 1.f;
    float TickInterval = 0.25f;
    int32 MaxTicks = 1;
    float Delay = 0.f;

    int32 CurrentTick = 0;
    FTimerHandle TickHandle;
    FTimerHandle DelayHandle;

    virtual void Execute(AURPCharacterBase* Owner) override;
    
    virtual void DoAOETick();
    
    void StartTick();
};