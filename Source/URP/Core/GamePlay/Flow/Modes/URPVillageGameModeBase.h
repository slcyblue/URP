// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Types/URPPlayerTypes.h"
#include "Core/GamePlay/Flow/World/URPMonsterPool.h"
#include "Characters/Monster/URPMonsterCharacter.h"
#include "URPVillageGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class URP_API AURPVillageGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

    AURPVillageGameModeBase();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable)
    void InitFromCachedPlayerData();

private:
    void SpawnFromDataAsync(APlayerController* PC, const FPlayerData& Types);
    void OnPawnClassLoaded(FSoftObjectPath SoftPath, APlayerController* PC, FPlayerData Types);

    FDelegateHandle PendingLoadHandle;

public:
    UPROPERTY()
    TArray<class AURPMonsterSpawnZone*> SpawnZones;

private:
    FTimerHandle SpawnTimerHandle;

    void InitializeSpawnZones();
    void SpawnTick();

    UURPMonsterPool* MonsterPool;
};
