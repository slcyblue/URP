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

    /** 서버에서 데이터 동기화 요청 */
    UFUNCTION(BlueprintCallable)
    void StartDataSync();

    /** 동기화 완료 시 호출되는 이벤트 */
    UPROPERTY(BlueprintAssignable)
    FOnDataSyncComplete OnDataSyncComplete;

private:
    /** GameDataManager 캐시 */
    UPROPERTY()
    UURPGameDataManager* GameDataManager;

    /** 네트워크 허브 캐시 */
    UPROPERTY()
    class AURPNetworkManager* NetworkManager;

    /** 데이터 응답 처리 */
    UFUNCTION()
    void HandleDataReceived(const TArray<FGameDataPacket>& DataArray);
};
