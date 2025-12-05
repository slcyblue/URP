#pragma once

#include "CoreMinimal.h"
#include "Characters/Common/URPCharacterBase.h"
#include "Components/SphereComponent.h"
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
    virtual void HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    // 어그로 감지 범위 (동적 설정)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aggro")
    float AggroRadius = 600.f;

    // 실제 감지 콜리전
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aggro")
    USphereComponent* AggroSphere;

    UFUNCTION()
    void OnAggroEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    // Init
    void InitializeFromMonsterData(const FString MonsterName, int32 DifficultyLevel);
    void SetActive(bool bActive);
    bool IsActive() const { return bIsActive; }

    // Target
    void SetTarget(AActor* NewTargetActor);
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
