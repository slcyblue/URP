#include "URPNetworkSubsystem.h"
#include "Core/Subsystems/URPGameDataSubsystem.h"
#include "Network/LoginRPCHandler.h"
#include "Network/GameDataRPCHandler.h"
#include "Network/PlayerDataRPCHandler.h"
#include "Server/URPServerDataService.h"
#include "Kismet/GameplayStatics.h"

void UURPNetworkSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    auto RegisterHandler = [&](UClass* HandlerClass)
        {
            UBaseRPCHandler* NewHandler = Cast<UBaseRPCHandler>(NewObject<UObject>(this, HandlerClass));
            if (NewHandler)
            {
                NewHandler->Initialize(this);
                HandlerMap.Add(HandlerClass, NewHandler);
                UE_LOG(LogTemp, Log, TEXT("[NetworkSubsystem] Registered Handler: %s"), *HandlerClass->GetName());
            }
        };

    RegisterHandler(ULoginRPCHandler::StaticClass());
    RegisterHandler(UPlayerDataRPCHandler::StaticClass());
    RegisterHandler(UGameDataRPCHandler::StaticClass());

    UE_LOG(LogTemp, Log, TEXT("[NetworkSubsystem] All RPCHandlers registered."));


    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("[NetworkSubsystem] No valid world."));
        return;
    }

    // GetAuthGameMode()가 있으면 서버 (Listen 또는 Dedicated)
    if (World->GetAuthGameMode())
    {
        bIsListenServer = !IsRunningDedicatedServer();

        if (bIsListenServer)
        {
            UURPServerDataService::Initialize();
            UE_LOG(LogTemp, Log, TEXT("[NetworkSubsystem] Listen Server detected - skip handshake."));
            CheckGameDataVersion();  // 바로 버전체크
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("[NetworkSubsystem] Dedicated Server detected - waiting for clients."));
        }
    }
    else
    {
        // 일반 클라이언트 → 서버 접속 필요
        ConnectToServer();
    }

    // 필요 시 주기 동기화 시작
    // StartPeriodicSync(30.0f);
}

void UURPNetworkSubsystem::Deinitialize()
{
    //StopPeriodicSync();
    Super::Deinitialize();
}


bool UURPNetworkSubsystem::ConnectToServer()
{
    // TODO: 외부 접속 클라이언트용 세션 연결 추가
    UE_LOG(LogTemp, Log, TEXT("[NetworkSubsystem] Client connecting to Listen Server..."));
    return true;
}

void UURPNetworkSubsystem::CheckGameDataVersion()
{
    /*if (!GameDataHandler)
    {
        UE_LOG(LogTemp, Warning, TEXT("[NetworkSubsystem] GameDataHandler not ready."));
        return;
    }

    FString LocalVersion = UURPGameDataManager::Get()->GetCurrentVersion();
    FGameDataSyncRequest Req;
    Req.ClientVersion = LocalVersion;

    UE_LOG(LogTemp, Log, TEXT("[NetworkSubsystem] Requesting GameData version check (%s)"), *LocalVersion);
    GameDataHandler->Server_RequestVersionDiff(Req);*/
}

#pragma region DataSync
void UURPNetworkSubsystem::StartPeriodicSync(float IntervalSec)
{
    if (UWorld* W = GetWorld())
    {
        W->GetTimerManager().SetTimer(
            SyncTimerHandle, this, &UURPNetworkSubsystem::TickSync,
            IntervalSec, true, IntervalSec);
    }
}

void UURPNetworkSubsystem::StopPeriodicSync()
{
    if (UWorld* W = GetWorld())
    {
        W->GetTimerManager().ClearTimer(SyncTimerHandle);
    }
}

void UURPNetworkSubsystem::TickSync()
{
    //if (GameDataHandler)
    //{
    //    GameDataHandler->Client_RequestVersionDiff(); // 클라→서버: 현재 버전 보내고 차이 받기
    //}
}
#pragma endregion