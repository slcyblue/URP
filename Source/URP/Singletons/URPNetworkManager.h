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

    /** �ڵ鷯 ������ */
    class UGameDataRPCHandler* GetDataHandler() const { return GameDataHandler; }
    
    
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString PlayerId;  // �α��� ���� �� �������� ���

    void SetAuthenticatedPlayerId(const FString& InId) { PlayerId = InId; }
    FString GetAuthenticatedPlayerId() const { return PlayerId; }


    /** �ڵ鷯 */
    UPROPERTY()
    class ULoginRPCHandler* LoginHandler;
protected:
    /** �ý��ۺ� RPC �ڵ鷯 */
    UPROPERTY() class UGameDataRPCHandler* GameDataHandler;
};