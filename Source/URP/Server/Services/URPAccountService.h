#pragma once

#include "CoreMinimal.h"
#include "Server/Core/URPServerServiceBase.h"
#include "Server/Storage/URPJsonStorage.h"
#include "Types/URPLoginTypes.h"
#include "UObject/NoExportTypes.h"
#include "URPAccountService.generated.h"

UCLASS()
class URP_API UURPAccountService : public UURPServerServiceBase
{
    GENERATED_BODY()

public:
    UURPAccountService();

    /** 단일 진입점 (Login) */
    bool OnLogin(const void* Payload, void* OutResponse);

protected:
    /** 내부 처리 로직 */
    bool CreateAccount(const FLoginRequest& Req, FLoginResponse& Out);
    bool VerifyLogin(const FLoginRequest& Req, FLoginResponse& Out);
};
