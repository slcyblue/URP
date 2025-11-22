#pragma once
#include "CoreMinimal.h"
#include "URPSkillBase.h"
#include "URPProjectileBase.generated.h"

UCLASS()
class URP_API UURPProjectileBase : public UURPSkillBase
{
    GENERATED_BODY()


public:
    TSubclassOf<AActor> ProjectileClass;
    float DamageMultiplier = 1.f;
    float Range = 0.f;
    FVector SpawnOffset;

    virtual void Execute(AURPPlayerCharacter* Owner) override;
};
