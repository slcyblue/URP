#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/URPPlayerData.h"
#include "BaseRPCHandler.h"
#include "PlayerDataRPCHandler.generated.h"


/** 유저 데이터 응답 이벤트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDataResponse, const FPlayerDataResponse&, PlayerData);

UCLASS()
class URP_API UPlayerDataRPCHandler : public UBaseRPCHandler
{
    GENERATED_BODY()

public:
    virtual void Initialize(class UURPNetworkSubsystem* InNetwork) override;

    UFUNCTION(Server, Reliable)
    void Server_CreatePlayer(const FString& PlayerId, EURPClassType ClassType);

    UFUNCTION(Server, Reliable)
    void Server_RequestPlayerData(const FPlayerDataRequest& Request);

    UFUNCTION(Client, Reliable)
    void Client_ReceivePlayerData(const FPlayerDataResponse& Response);

    /** 플레이어 데이터 응답 델리게이트 */
    UPROPERTY(BlueprintAssignable)
    FOnPlayerDataResponse OnPlayerDataResponse;
};