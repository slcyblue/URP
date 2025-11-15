#pragma once

#include "CoreMinimal.h"
#include "URPCharacterBase.h"
#include "Animations/URPPlayerAnimInstance.h"
#include "URPGameData.h"
#include "GamePlay/URPMonsterSpawnZone.h"
#include "URPMonsterCharacter.generated.h"

/**
 * 플레이어 전용 캐릭터 클래스
 * - 입력 처리
 * - 카메라 제어
 * - UI 연동
 */
UCLASS()
class URP_API AURPMonsterCharacter : public AURPCharacterBase
{
    GENERATED_BODY()

public:
    AURPMonsterCharacter();
    
    virtual void Die();                     // 사망 처리 함수
    virtual void OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    void SetActive(bool bActive);
    bool IsActive() const { return bIsActive; }

    void InitializeFromMonsterData(const FString MonsterName, int32 DifficultyLevel);

    UPROPERTY()
    AURPMonsterSpawnZone* OwningZone;

protected:
    virtual void BeginPlay() override;

private:
    bool bIsActive = false;

    void ApplyAppearance(const FURPMonsterRow& Data, const FURPPathConfig& Path, int32 DifficultyLevel);
    void ApplyStats(const FURPMonsterRow& Data, int32 DifficultyLevel);
    void ApplyAI(const FURPPathConfig& Path);

    /** 몬스터 Death 몽타주 */
    UPROPERTY(EditAnywhere, Category = "Animation")
    UAnimMontage* DeathMontage;
};
