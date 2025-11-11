// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/URPPlayerData.h"
#include "URPVillageGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class URP_API AURPVillageGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable)
    void InitFromCachedPlayerData();

private:
    void SpawnFromDataAsync(APlayerController* PC, const FPlayerData& Data);
    void OnPawnClassLoaded(FSoftObjectPath SoftPath, APlayerController* PC, FPlayerData Data);

    FDelegateHandle PendingLoadHandle;
};
