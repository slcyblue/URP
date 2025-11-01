#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LoginRPCHandler.generated.h"

USTRUCT(BlueprintType)
struct FLoginRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Password;
};

USTRUCT(BlueprintType)
struct FLoginResponse
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Message;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerId;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginResponse, const FLoginResponse&, Result);

UCLASS()
class URP_API ULoginRPCHandler : public UObject
{
    GENERATED_BODY()

public:
    void Init(class AURPNetworkManager* InOwner);

    /** Ŭ�� �� ���� : �α��� ��û */
    UFUNCTION(Server, Reliable)
    void Server_LoginRequest(const FLoginRequest& Request);

    /** ���� �� Ŭ�� : �α��� ���� */
    UFUNCTION(Client, Reliable)
    void Client_LoginResponse(const FLoginResponse& Response);

    /** �α��� ��� �̺�Ʈ */
    UPROPERTY(BlueprintAssignable)
    FOnLoginResponse OnLoginResponse;

private:
    UPROPERTY()
    AURPNetworkManager* Owner;
};