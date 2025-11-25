#include "LoginRPCHandler.h"
#include "Core/Subsystems/Network/URPNetworkSubsystem.h"
#include "Core/Subsystems/Data/URPUserDataSubsystem.h"
#include "Network/PlayerDataRPCHandler.h"
#include "Server/URPServerDataService.h"
#include "Server/Services/URPAccountService.h"


void ULoginRPCHandler::Initialize(UURPNetworkSubsystem* InNetwork)
{
    Super::Initialize(InNetwork);
}

void ULoginRPCHandler::Server_LoginRequest_Implementation(const FLoginRequest& Request)
{
    FLoginResponse Response;
    if (UURPServerDataService* ServerData = UURPServerDataService::Get())
    {
        ServerData->RouteRequest(EURPServerRequestType::Login, &Request, &Response);
    }

    Client_LoginResponse(Response);
}

void ULoginRPCHandler::Client_LoginResponse_Implementation(const FLoginResponse& Response)
{
    UE_LOG(LogTemp, Log, TEXT("[LoginRPCHandler] Login Response: %s"), *Response.Message);

    // Delegate 브로드캐스트
    OnLoginResponse.Broadcast(Response);

    GetSubsystem<UURPUserDataSubsystem>()->SetCurrentPlayerId(Response.PlayerId);
}