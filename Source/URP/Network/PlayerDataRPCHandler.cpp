#include "PlayerDataRPCHandler.h"
#include "Core/Managers/URPUserDataManager.h"
#include "../Core/Subsystems/URPNetworkSubsystem.h"
#include "../Server/URPServerDataService.h"
#include "../Server/Services/URPPlayerDataService.h"
#include <Kismet/GameplayStatics.h>

void UPlayerDataRPCHandler::Initialize(UURPNetworkSubsystem* InNetwork)
{
    Super::Initialize(InNetwork);
}

void UPlayerDataRPCHandler::Server_CreatePlayer_Implementation(const FString& PlayerId, EURPClassType ClassType)
{
    FPlayerDataRequest Req;
    Req.Action = TEXT("Init");
    Req.PlayerId = PlayerId;
    Req.PlayerData.PlayerId = PlayerId;
    Req.PlayerData.SelectedClass = ClassType;

    FPlayerDataResponse Res;
    if (UURPServerDataService* ServerData = UURPServerDataService::Get())
    {
        ServerData->RouteRequest(EURPServerRequestType::CreatePlayer, &Req, &Res);
    }

    Client_ReceivePlayerData(Res);
}

void UPlayerDataRPCHandler::Server_RequestPlayerData_Implementation(const FPlayerDataRequest& Request)
{
    FPlayerDataResponse Response;
    Response.bSuccess = false;
    Response.Action = Request.Action;   // <- 돌려주기(구분용)

    if (UURPServerDataService* ServerData = UURPServerDataService::Get())
    {
        if (Request.Action == TEXT("Load"))
            ServerData->RouteRequest(EURPServerRequestType::LoadPlayer, &Request, &Response);
        else if (Request.Action == TEXT("Save"))
            ServerData->RouteRequest(EURPServerRequestType::SavePlayer, &Request, &Response);
        else
            Response.Message = TEXT("Invalid Action");
    }

    Client_ReceivePlayerData(Response);
}

void UPlayerDataRPCHandler::Client_ReceivePlayerData_Implementation(const FPlayerDataResponse& Response)
{
    UE_LOG(LogTemp, Log, TEXT("[PlayerDataRPCHandler] Received player data response: %s"), *Response.Message);
    OnPlayerDataResponse.Broadcast(Response);

    if (!Response.bSuccess)
        return;

    if (Response.Action == TEXT("Load"))
    {
        if (auto* UserDataManager = UURPUserDataManager::Get())
        {
            UserDataManager->SetUserData(Response.PlayerData);
        }
    }
}