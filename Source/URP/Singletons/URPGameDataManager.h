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

    /** ����: /Content/Data �� JSON �ε� */
    void LoadAllFromServerDisk();

    /** Ŭ���̾�Ʈ: ĳ�� ������ �ε� */
    void LoadLocalCache();

    /** Ŭ���̾�Ʈ: ĳ�� ���� */
    void SaveLocalCache();

    /** Ŭ���̾�Ʈ/���� ����: ������ ���� */
    void ApplyData(const FString& DataName, const FString& JsonString);

    /** ����: ��� JSON �� ��Ŷ �迭�� ��ȯ */
    void GetAllDataPackets(TArray<FGameDataPacket>& OutPackets) const;

    /** Ư�� ������ ��ȯ */
    FString GetDataAsString(FString DataName) const;

    /** ����׿�: ���� �ε�� ������ �α� ��� */
    void PrintSummary() const;

private:
    /** ���� �ε�� JSON ������ */
    UPROPERTY()
    TMap<FString, FString> CachedJson; // DataName �� JsonString
};
