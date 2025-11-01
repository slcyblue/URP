#include "LoginRPCHandler.h"
#include "../Singletons/URPNetworkManager.h"

void ULoginRPCHandler::Init(AURPNetworkManager* InOwner)
{
    Owner = InOwner;
}

void ULoginRPCHandler::Server_LoginRequest_Implementation(const FLoginRequest& Request)
{
    FLoginResponse Response;

    // 간단한 ID/PW 검증 로직 (임시)
    if (Request.ID == "test" && Request.Password == "1234")
    {
        Response.bSuccess = true;
        Response.Message = TEXT("Login successful!");
        Response.PlayerId = Request.ID;

        // 서버의 NetworkManager에 인증 정보 등록
        if (Owner)
            Owner->SetAuthenticatedPlayerId(Request.ID);
    }
    else
    {
        Response.bSuccess = false;
        Response.Message = TEXT("Invalid ID or Password");
        Response.PlayerId = TEXT("");
    }

    // 클라이언트로 응답 전송
    Client_LoginResponse(Response);
}

void ULoginRPCHandler::Client_LoginResponse_Implementation(const FLoginResponse& Response)
{
    UE_LOG(LogTemp, Log, TEXT("[LoginRPCHandler] Login Result: %s"), *Response.Message);
    OnLoginResponse.Broadcast(Response);
}