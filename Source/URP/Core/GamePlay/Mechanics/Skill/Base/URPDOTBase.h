#pragma once
#include "CoreMinimal.h"
#include "URPSkillBase.h"
#include "URPDOTBase.generated.h"

UCLASS()
class URP_API UURPDOTBase : public UURPSkillBase
{
    GENERATED_BODY()

public:
    UPROPERTY() float TickDamage = 0.f;
    UPROPERTY() float TickInterval = 1.f;
    UPROPERTY() float Duration = 5.f;

    void ApplyDOT(AURPCharacterBase* Target);

    virtual void Execute(AURPCharacterBase* Owner) override;
};
