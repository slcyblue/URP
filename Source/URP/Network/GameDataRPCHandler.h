#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Types/URPGameTypes.h"
#include "BaseRPCHandler.h"
#include "GameDataRPCHandler.generated.h"


/** 서버 → 클라 : 데이터 응답용 델리게이트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameDataSyncResponse, const FGameDataSyncResponse&, Response);

UCLASS()
class URP_API UGameDataRPCHandler : public UBaseRPCHandler
{
    GENERATED_BODY()

public:
    virtual void Initialize(class UURPNetworkSubsystem* InNetwork) override;

    /** 클라 → 서버 : 버전 동기화 요청 */
    UFUNCTION(Server, Reliable)
    void Server_RequestGameDataSync(const FGameDataSyncRequest& Request);

    /** 서버 → 클라 : 최신 데이터 응답 */
    UFUNCTION(Client, Reliable)
    void Client_ReceiveGameDataSync(const FGameDataSyncResponse& Response);

    /** 데이터 응답 델리게이트 */
    UPROPERTY(BlueprintAssignable)
    FOnGameDataSyncResponse OnGameDataSyncResponse;
};