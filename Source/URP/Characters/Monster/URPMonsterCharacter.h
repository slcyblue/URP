#pragma once

#include "CoreMinimal.h"
#include "Characters/Common/URPCharacterBase.h"
#include "URPGameTypes.h"
#include "URPMonsterCharacter.generated.h"

class AURPMonsterAIController;
class AURPMonsterSpawnZone;

UCLASS()
class URP_API AURPMonsterCharacter : public AURPCharacterBase
{
    GENERATED_BODY()

public:
    AURPMonsterCharacter();

    virtual void BeginPlay() override;
    virtual void Die();

    void InitializeFromMonsterData(const FString MonsterName, int32 DifficultyLevel);
    void SetActive(bool bActive);
    bool IsActive() const { return bIsActive; }

    // Target
    void SetTargetFromBlackboard(AActor* NewTargetActor);
    void ClearTarget();
    UFUNCTION()
    void OnTargetDied(AURPCharacterBase* Dead);

    // Attack
    void PerformBasicAttack(AActor* TargetActor);

    UPROPERTY()
    AURPCharacterBase* CurrentTarget = nullptr;

    UPROPERTY()
    AURPMonsterSpawnZone* OwningZone = nullptr;

private:
    bool bIsActive = false;

    void ApplyAppearance(const FURPMonsterRow& Data, const FURPPathConfig& Path, int32 DifficultyLevel);
    void ApplyStats(const FURPMonsterRow& Data, int32 DifficultyLevel);
};
