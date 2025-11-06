#include "Server/Services/URPAccountService.h"
#include "Server/Storage/URPJsonStorage.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonSerializer.h"

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
    FString PathCategory = TEXT("Accounts");

    // 중복 방지
    if (Storage->Exists(PathCategory, Req.ID))
    {
        Out.bSuccess = false;
        Out.Message = TEXT("Account already exists");
        return false;
    }

    // 계정 데이터 저장
    Out.PlayerId = Req.ID;
    Out.bSuccess = true;
    Out.Message = TEXT("New account created successfully");
    Out.bIsNewAccount = true;

    const bool bSaved = Storage->SaveStruct(PathCategory, Req.ID, &Out, FLoginResponse::StaticStruct());
    UE_LOG(LogTemp, Log, TEXT("[AccountService] Created account %s (%s)"), *Req.ID, bSaved ? TEXT("Saved") : TEXT("Save Failed"));

    return bSaved;
}

/**
 * 기존 계정 검증
 */
bool UURPAccountService::VerifyLogin(const FLoginRequest& Req, FLoginResponse& Out)
{
    FString PathCategory = TEXT("Accounts");

    if (!Storage->Exists(PathCategory, Req.ID))
    {
        Out.bSuccess = false;
        Out.Message = TEXT("Account not found");
        return false;
    }

    FLoginResponse Saved;
    const bool bLoaded = Storage->LoadStruct(PathCategory, Req.ID, &Saved, FLoginResponse::StaticStruct());
    if (!bLoaded)
    {
        Out.bSuccess = false;
        Out.Message = TEXT("Failed to load account data");
        return false;
    }

    // 패스워드 검증 (패스워드를 별도 저장하지 않았으면 일단 통과)
    if (Req.Password.IsEmpty() || Req.Password == Saved.Message)
    {
        Out = Saved;
        Out.bSuccess = true;
        Out.Message = TEXT("Login successful");
        Out.bIsNewAccount = false;
        UE_LOG(LogTemp, Log, TEXT("[AccountService] Login success: %s"), *Req.ID);
        return true;
    }

    Out.bSuccess = false;
    Out.Message = TEXT("Invalid password");
    UE_LOG(LogTemp, Warning, TEXT("[AccountService] Invalid password for %s"), *Req.ID);
    return false;
}
