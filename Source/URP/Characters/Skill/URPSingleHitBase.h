#pragma once
#include "CoreMinimal.h"
#include "URPSkillBase.h"
#include "URPSingleHitBase.generated.h"

UCLASS()
class URP_API UURPSingleHitBase : public UURPSkillBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    float Range = 200.f;

    UPROPERTY(EditAnywhere)
    float DamageMultiplier = 1.0f;

    UPROPERTY(EditAnywhere)
    float Radius = 50.f;

    virtual void Execute(AURPPlayerCharacter* Owner) override;
};