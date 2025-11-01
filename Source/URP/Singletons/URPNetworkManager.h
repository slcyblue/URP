#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "URPNetworkManager.generated.h"

UCLASS()
class URP_API AURPNetworkManager : public AActor
{
    GENERATED_BODY()

public:
    AURPNetworkManager();

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    /** 핸들러 접근자 */
    class UGameDataRPCHandler* GetDataHandler() const { return GameDataHandler; }
    
    
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString PlayerId;  // 로그인 성공 후 서버에서 등록

    void SetAuthenticatedPlayerId(const FString& InId) { PlayerId = InId; }
    FString GetAuthenticatedPlayerId() const { return PlayerId; }


    /** 핸들러 */
    UPROPERTY()
    class ULoginRPCHandler* LoginHandler;
protected:
    /** 시스템별 RPC 핸들러 */
    UPROPERTY() class UGameDataRPCHandler* GameDataHandler;
};