// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Singletons/URPGameDataManager.h"
#include "URPGameDataSubsystem.generated.h"
/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDataSyncComplete);

UCLASS()
class URP_API UURPGameDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    /** �������� ������ ����ȭ ��û */
    UFUNCTION(BlueprintCallable)
    void StartDataSync();

    /** ����ȭ �Ϸ� �� ȣ��Ǵ� �̺�Ʈ */
    UPROPERTY(BlueprintAssignable)
    FOnDataSyncComplete OnDataSyncComplete;

private:
    /** GameDataManager ĳ�� */
    UPROPERTY()
    UURPGameDataManager* GameDataManager;

    /** ��Ʈ��ũ ��� ĳ�� */
    UPROPERTY()
    class AURPNetworkManager* NetworkManager;

    /** ������ ���� ó�� */
    UFUNCTION()
    void HandleDataReceived(const TArray<FGameDataPacket>& DataArray);
};
