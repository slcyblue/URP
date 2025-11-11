// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/URPGameData.h"
#include "URPGameDataSubsystem.generated.h"
/**
 * 
 */

UCLASS()
class URP_API UURPGameDataSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

public:
    /** Subsystem 초기화 */
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    /** 서버 버전체크 및 데이터 요청 */
    void InitializeGameData();


    FString GetCurrentVersion() const { return CachedVersion; }

    UFUNCTION()
    void ApplyServerUpdate(const FGameDataSyncResponse& Response);

    TOptional<FString> GetPawnPathByClass(EURPClassType ClassType) const;

private:
    static UURPGameDataSubsystem* Instance;

    FString CachedVersion;
    TArray<FGameDataPacket> GameTables;


    TArray<FURPMonsterRow> MonsterTable;
    //TArray<FURPItemRow> ItemTable;
    FCharacterPresetTable PresetTable;

    void LoadLocalFallback(); // Content/ 또는 Saved/캐시에서 읽기
};
