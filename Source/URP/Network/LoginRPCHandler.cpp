#include "LoginRPCHandler.h"
#include "../Core/Subsystems/URPNetworkSubsystem.h"
#include "../Network/PlayerDataRPCHandler.h"
#include "../Server/URPServerDataService.h"
#include "../Server/Services/URPAccountService.h"


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

    if (!Response.bSuccess)
    {
        // TODO: 에러 UI
        return;
    }

    // 신규 계정 → 캐릭터 선택 화면으로만 이동 (PlayerData 요청 금지)
    if (Response.bIsNewAccount)
    {
        return;
    }

    // 기존 계정 → PlayerData Load
    if (auto* PlayerHandler = NetworkSubsystem->GetHandler<UPlayerDataRPCHandler>())
    {
        FPlayerDataRequest Request;
        Request.PlayerId = Response.PlayerId;
        Request.Action = TEXT("Load");
        UE_LOG(LogTemp, Log, TEXT("[LoginRPCHandler] Existing account -> Requesting PlayerData(Load) for %s"), *Request.PlayerId);
        PlayerHandler->Server_RequestPlayerData(Request);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[LoginRPCHandler] PlayerDataRPCHandler not found in NetworkSubsystem."));
    }
}