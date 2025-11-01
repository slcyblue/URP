#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../Singletons/URPGameDataManager.h"
#include "GameDataRPCHandler.generated.h"

UCLASS()
class URP_API UGameDataRPCHandler : public UObject
{
    GENERATED_BODY()

public:
    void Init(class AURPNetworkManager* InOwner);

    UFUNCTION(Server, Reliable)
    void Server_RequestAllData();

    UFUNCTION(Client, Reliable)
    void Client_ReceiveAllData(const TArray<FGameDataPacket>& DataArray);

private:
    UPROPERTY() AURPNetworkManager* Owner;
};