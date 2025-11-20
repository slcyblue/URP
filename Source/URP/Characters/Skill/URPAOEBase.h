#pragma once
#include "CoreMinimal.h"
#include "URPSkillBase.h"
#include "URPAOEBase.generated.h"

UCLASS()
class URP_API UURPAOEBase : public UURPSkillBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    float Radius = 300.f;

    UPROPERTY(EditAnywhere)
    float DamageMultiplier = 1.0f;

    virtual void Execute(AURPPlayerCharacter* Owner) override;
};