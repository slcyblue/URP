// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Datas/URPGameDataTableBase.h"
#include "GameFramework/PlayerController.h"
#include "URPGameDataManager.generated.h"
/**
 * 
 */
USTRUCT()
struct FGameDataPacket
{
    GENERATED_BODY()

    UPROPERTY()
    FString DataName;

    UPROPERTY()
    FString JsonString;
};

UCLASS()
class URP_API UURPGameDataManager : public UObject
{
    GENERATED_BODY()

public:
    static UURPGameDataManager* Get(UWorld* World);

    void Initialize(UWorld* World);

    /** 서버: /Content/Data 내 JSON 로드 */
    void LoadAllFromServerDisk();

    /** 클라이언트: 캐시 데이터 로드 */
    void LoadLocalCache();

    /** 클라이언트: 캐시 저장 */
    void SaveLocalCache();

    /** 클라이언트/서버 공용: 데이터 적용 */
    void ApplyData(const FString& DataName, const FString& JsonString);

    /** 서버: 모든 JSON → 패킷 배열로 변환 */
    void GetAllDataPackets(TArray<FGameDataPacket>& OutPackets) const;

    /** 특정 데이터 반환 */
    FString GetDataAsString(FString DataName) const;

    /** 디버그용: 현재 로드된 데이터 로그 출력 */
    void PrintSummary() const;

private:
    /** 현재 로드된 JSON 데이터 */
    UPROPERTY()
    TMap<FString, FString> CachedJson; // DataName → JsonString
};
