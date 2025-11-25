#pragma once

#include "CoreMinimal.h"
#include "URPSkillBase.h"
#include "URPDebuffBase.generated.h"

UCLASS()
class URP_API UURPDebuffBase : public UURPSkillBase
{
    GENERATED_BODY()

public:

    UPROPERTY() EURPDebuffType DebuffType;
    UPROPERTY() float DebuffValue;
    UPROPERTY() float Duration;

    virtual void Execute(AURPCharacterBase* Owner) override;
};
