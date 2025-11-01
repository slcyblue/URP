#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../Datas/URPPlayerDataTypes.h"
#include "PlayerDataRPCHandler.generated.h"



UCLASS()
class URP_API UPlayerDataRPCHandler : public UObject
{
    GENERATED_BODY()

public:
    void Init(class AURPNetworkManager* InOwner);

    /** 클라 → 서버 : 플레이어 데이터 요청 */
    UFUNCTION(Server, Reliable)
    void Server_RequestPlayerData(const FString& PlayerId);

    /** 서버 → 클라 : 데이터 전송 */
    UFUNCTION(Client, Reliable)
    void Client_ReceivePlayerData(const FPlayerCharacterData& CharData,
        const FPlayerInventoryData& InvData,
        const FPlayerSkillData& SkillData);

    /** 클라 → 서버 : 데이터 저장 */
    UFUNCTION(Server, Reliable)
    void Server_SavePlayerData(const FPlayerCharacterData& CharData,
        const FPlayerInventoryData& InvData,
        const FPlayerSkillData& SkillData);

private:
    UPROPERTY()
    AURPNetworkManager* Owner;
};