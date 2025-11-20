#pragma once
#include "CoreMinimal.h"
#include "URPSkillBase.h"
#include "URPProjectileBase.generated.h"

UCLASS()
class URP_API UURPProjectileBase : public UURPSkillBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> ProjectileClass;

    UPROPERTY(EditAnywhere)
    FVector SpawnOffset = FVector(30, 0, 50);

    UPROPERTY(EditAnywhere)
    float DamageMultiplier = 1.0f;

    virtual void Execute(AURPPlayerCharacter* Owner) override;
};
