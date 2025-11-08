#include "Server/Services/URPAccountService.h"
#include "Server/Storage/URPJsonStorage.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"
#include "Data/URPPlayerData.h"
#include <URPServerDataService.h>

UURPAccountService::UURPAccountService()
{
    // Login 요청만 등록
    RegisterHandler<UURPAccountService>(EURPServerRequestType::Login, &UURPAccountService::OnLogin);
}

/**
 * 로그인 요청 진입점
 * - 계정 존재 여부 확인
 * - 신규면 CreateAccount()
 * - 기존이면 VerifyLogin()
 */
bool UURPAccountService::OnLogin(const void* Payload, void* OutResponse)
{
    const FLoginRequest* Req = static_cast<const FLoginRequest*>(Payload);
    FLoginResponse* Res = static_cast<FLoginResponse*>(OutResponse);
    if (!Req || !Res)
        return false;

    UE_LOG(LogTemp, Log, TEXT("[AccountService] Login request received: %s"), *Req->ID);

    // 계정 존재 여부 확인
    const bool bExists = Storage->Exists(TEXT("Accounts"), Req->ID);

    if (!bExists)
    {
        UE_LOG(LogTemp, Log, TEXT("[AccountService] Account not found, creating new one for %s"), *Req->ID);
        return CreateAccount(*Req, *Res);
    }

    UE_LOG(LogTemp, Log, TEXT("[AccountService] Account exists, verifying %s"), *Req->ID);
    return VerifyLogin(*Req, *Res);
}

/**
 * 신규 계정 생성
 */
bool UURPAccountService::CreateAccount(const FLoginRequest& Req, FLoginResponse& Out)
{
    // 1. 계정 저장
    FAccountData Account;
    Account.AccountId = Req.ID;
    Account.Password = Req.Password;
    Storage->SaveStruct(TEXT("Accounts"), Req.ID, &Account, FAccountData::StaticStruct());

    // 2. 기본 PlayerData 생성 (SelectedClass = None)
    if (UURPServerDataService* ServerData = UURPServerDataService::Get())
    {
        FPlayerDataRequest PlayerReq;
        PlayerReq.PlayerId = Req.ID;

        FPlayerDataResponse PlayerRes;
        ServerData->RouteRequest(EURPServerRequestType::CreatePlayer, &PlayerReq, &PlayerRes);
    }

    Out.bSuccess = true;
    Out.Message = TEXT("New account created");
    Out.PlayerId = Req.ID;
    Out.SelectedClass = EURPClassType::None;
    return true;
}

/**
 * 기존 계정 검증
 */
bool UURPAccountService::VerifyLogin(const FLoginRequest& Req, FLoginResponse& Out)
{
    FAccountData Account;
    if (!Storage->LoadStruct(TEXT("Accounts"), Req.ID, &Account, FAccountData::StaticStruct()))
    {
        Out.bSuccess = false;
        Out.Message = TEXT("Account load failed");
        return false;
    }

    if (Account.Password != Req.Password)
    {
        Out.bSuccess = false;
        Out.Message = TEXT("Invalid password");
        return false;
    }

    Out.bSuccess = true;
    Out.PlayerId = Req.ID;
    Out.Message = TEXT("Login successful");
    return true;
}
