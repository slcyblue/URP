// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/URPGameData.h"
#include "URPGameDataManager.generated.h"
/**
 * 
 */

UCLASS()
class URP_API UURPGameDataManager : public UObject
{
    GENERATED_BODY()

public:
    static UURPGameDataManager* Get();

    void Initialize(); // 로컬 기본 로드 + 서버 버전체크 트리거

    FString GetCurrentVersion() const { return CachedVersion; }

    void ApplyServerUpdate(const FString& NewVersion, const TArray<FGameDataPacket>& Tables);

    TOptional<FString> GetPawnPathByClass(EURPClassType ClassType) const;

private:
    static UURPGameDataManager* Instance;

    FString CachedVersion;
    TArray<FGameDataPacket> GameTables;

    TArray<FURPMonsterRow> MonsterTable;
    //TArray<FURPItemRow> ItemTable;
    FCharacterPresetTable PresetTable;

    void LoadLocalFallback(); // Content/ 또는 Saved/캐시에서 읽기

};
