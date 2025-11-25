#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Types/URPLoginTypes.h"
#include "Types/URPPlayerTypes.h"
#include "BaseRPCHandler.h"
#include "LoginRPCHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginResponse, const FLoginResponse&, Result);

UCLASS()
class URP_API ULoginRPCHandler : public UBaseRPCHandler
{
    GENERATED_BODY()

public:
    virtual void Initialize(class UURPNetworkSubsystem* InNetwork) override;

    /** 클라 → 서버 : 로그인 요청 */
    UFUNCTION(Server, Reliable)
    void Server_LoginRequest(const FLoginRequest& Request);

    /** 서버 → 클라 : 로그인 결과 수신 */
    UFUNCTION(Client, Reliable)
    void Client_LoginResponse(const FLoginResponse& Response);

    UPROPERTY(BlueprintAssignable)
    FOnLoginResponse OnLoginResponse;
};