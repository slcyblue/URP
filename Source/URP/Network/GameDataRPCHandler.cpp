#include "GameDataRPCHandler.h"
#include "../Core/Subsystems/URPGameDataSubsystem.h"
#include "../Core/Subsystems/URPNetworkSubsystem.h"
#include "../Server/URPServerDataService.h"
#include "../Server/Services/URPGameDataService.h"

void UGameDataRPCHandler::Initialize(UURPNetworkSubsystem* InNetwork)
{
    Super::Initialize(InNetwork);
}

void UGameDataRPCHandler::Server_RequestGameDataSync_Implementation(const FGameDataSyncRequest& Request)
{
    UE_LOG(LogTemp, Log, TEXT("[GameDataRPCHandler] Server received sync request. Client version: %s"), *Request.ClientVersion);

    FGameDataSyncResponse Response;
    Response.bUpToDate = false;

    if (UURPServerDataService* ServerData = UURPServerDataService::Get())
    {
        ServerData->RouteRequest(EURPServerRequestType::SyncGameData, &Request, &Response);
    }

    // 서버 → 클라로 최신 데이터 전송
    Client_ReceiveGameDataSync(Response);
}

void UGameDataRPCHandler::Client_ReceiveGameDataSync_Implementation(const FGameDataSyncResponse& Response)
{
    UE_LOG(LogTemp, Log, TEXT("[GameDataRPCHandler] Client received game data sync. UpToDate: %d, NewVersion: %s"),
        Response.bUpToDate, *Response.NewVersion);

    OnGameDataSyncResponse.Broadcast(Response);

    UE_LOG(LogTemp, Log, TEXT("[GameDataRPCHandler] Received GameData Version: %s"), *Response.NewVersion);
}