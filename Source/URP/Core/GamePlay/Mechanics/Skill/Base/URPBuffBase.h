#pragma once

#include "CoreMinimal.h"
#include "URPSkillBase.h"
#include "URPBuffBase.generated.h"

UCLASS()
class URP_API UURPBuffBase : public UURPSkillBase
{
    GENERATED_BODY()

public:
    UPROPERTY() EURPBuffType BuffType;
    UPROPERTY() float BuffValue;
    UPROPERTY() float Duration;

    virtual void Execute(AURPCharacterBase* Owner) override;
};
