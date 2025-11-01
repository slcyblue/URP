// Fill out your copyright notice in the Description page of Project Settings.


#include "URPGameDataSubsystem.h"
#include "URPGameInstance.h"
#include "Singletons/URPNetworkManager.h"
#include "Networks/GameDataRPCHandler.h"
#include "Kismet/GameplayStatics.h"

void UURPGameDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    // 매니저 인스턴스 생성
    GameDataManager = UURPGameDataManager::Get(GetWorld());

    // GameInstance에서 NetworkManager 가져오기
    if (const UURPGameInstance* GI = Cast<UURPGameInstance>(GetGameInstance()))
    {
        NetworkManager = GI->GetNetworkManager();
    }

    if (!NetworkManager)
    {
        UE_LOG(LogTemp, Error, TEXT("[UURPGameDataSubsystem] NetworkManager not found!"));
    }
}

void UURPGameDataSubsystem::StartDataSync()
{
    if (!GameDataManager)
        GameDataManager = UURPGameDataManager::Get(GetWorld());

    if (!NetworkManager)
    {
        UE_LOG(LogTemp, Error, TEXT("[UURPGameDataSubsystem] Cannot sync data, NetworkManager is null"));
        return;
    }

    if (GetWorld()->GetAuthGameMode()) // 서버
    {
        GameDataManager->LoadAllFromServerDisk();
        OnDataSyncComplete.Broadcast();
        UE_LOG(LogTemp, Log, TEXT("[UURPGameDataSubsystem] Server loaded data locally."));
    }
    else // 클라이언트
    {
        GameDataManager->LoadLocalCache();

        // DataRPCHandler를 통해 서버에 요청
        UGameDataRPCHandler* Handler = NetworkManager->GetDataHandler();
        if (Handler)
        {
            UE_LOG(LogTemp, Log, TEXT("[UURPGameDataSubsystem] Requesting data from server..."));
            Handler->Server_RequestAllData();
        }
    }
}

void UURPGameDataSubsystem::HandleDataReceived(const TArray<FGameDataPacket>& DataArray)
{
    if (!GameDataManager)
        GameDataManager = UURPGameDataManager::Get(GetWorld());

    for (const auto& P : DataArray)
    {
        GameDataManager->ApplyData(P.DataName, P.JsonString);
    }
    GameDataManager->SaveLocalCache();

    UE_LOG(LogTemp, Log, TEXT("[UURPGameDataSubsystem] Data sync complete (%d entries)"), DataArray.Num());
    OnDataSyncComplete.Broadcast();
}