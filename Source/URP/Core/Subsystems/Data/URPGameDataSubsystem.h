// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Types/URPGameTypes.h"
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

    const FURPMonsterRow* GetMonsterRow(const FString ID);

    const FURPPathConfig& GetPathConfig() const { return PathConfig; }

    const TArray<FURPSkillRow>& GetSkillTable() const { return SkillTable; }

    const FURPClassData* GetClassData(EURPClassType ClassType);

private:
    static UURPGameDataSubsystem* Instance;

    FString CachedVersion;
    TArray<FGameDataPacket> GameTables;

    TArray<FURPMonsterRow> MonsterTable;
    FURPPathConfig PathConfig;
    TArray<FURPSkillRow> SkillTable;
    TArray<FURPClassData> ClassTable;
    //TArray<FURPItemRow> ItemTable;


    void LoadLocalFallback(); // Content/ 또는 Saved/캐시에서 읽기

    /** 테이블 이름 -> 로딩 함수 */
    TMap<FString, TFunction<void(const FGameDataPacket&)>> TableLoaders;

    // 각 테이블별 로딩 함수
    void LoadMonsterTable(const FGameDataPacket& Packet);
    void LoadPathConfig(const FGameDataPacket& Packet);
    void LoadSkillData(const FGameDataPacket& Packet);
    void LoadClassData(const FGameDataPacket& Packet);
};
