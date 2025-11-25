#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "URPMonsterPool.h"
#include "Types/URPGameTypes.h"
#include "URPMonsterSpawnZone.generated.h"

class USphereComponent;
class AURPMonsterCharacter;

UCLASS()
class URP_API AURPMonsterSpawnZone : public AActor
{
    GENERATED_BODY()

public:
    AURPMonsterSpawnZone();

    /* 존 난이도 설정 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnZone")
    int32 ZoneLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnZone")
    TArray<FString> MonsterName;

    UPROPERTY()
    UURPMonsterPool* MonsterPool;

protected:
    virtual void BeginPlay() override;

private:
    /* 플레이어 감지 트리거 */
    UPROPERTY(VisibleAnywhere)
    USphereComponent* ZoneTrigger;

    /* 존 활성 여부 */
    bool bIsActive = false;

    /* 스폰 반경 */
    UPROPERTY(EditAnywhere)
    float SpawnRadius = 600.f;

    /* 최대 몬스터 유지 수 */
    UPROPERTY(EditAnywhere)
    int32 MaxCount = 10;

    /* 스폰할 몬스터 BP */
    UPROPERTY(EditAnywhere)
    TSubclassOf<AURPMonsterCharacter> MonsterClass;

    /* 현재 살아있는 몬스터 목록 */
    UPROPERTY()
    TArray<AURPMonsterCharacter*> CurrentMonsters;

    /* 플레이어가 내부에 있는가? */
    bool bPlayerInside = false;

    /* 스폰 유지 타이머 */
    FTimerHandle SpawnTimer;

private:
    UFUNCTION()
    void OnPlayerEnterZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bSweep, const FHitResult& Hit);

    UFUNCTION()
    void OnPlayerExitZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 BodyIndex);

    /* 활성화/비활성화 */
    void ActivateZone();
    void DeactivateZone();

    FTimerHandle DeactivateDelayTimer;

    /* 스폰 로직 */
    void SpawnMissingMonsters();
    bool FindValidSpawnPoint(FVector& OutLocation);
    const FString GetRandomMonsterName() const;
    void ReturnMonsterToPool(AURPMonsterCharacter* Mob);
    void ReturnAllMonstersToPool();
};
