#pragma once
#include "CoreMinimal.h"
#include "Core/GamePlay/Mechanics/Skill/Base/URPSkillBase.h"
#include "URPDashBase.generated.h"

UCLASS()
class URP_API UURPDashBase : public UURPSkillBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    float DashDistance = 600.f;

    UPROPERTY(EditAnywhere)
    float DamageMultiplier = 1.f;

    UPROPERTY(EditAnywhere)
    float HitRadius = 60.f;

    virtual void Execute(AURPCharacterBase* Owner) override;
};