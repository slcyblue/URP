#pragma once
#include "CoreMinimal.h"
#include "Core/GamePlay/Mechanics/Skill/Base/URPSkillBase.h"
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

    virtual void Execute(AURPCharacterBase* Owner) override;
};