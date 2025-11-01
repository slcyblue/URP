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

    /** Ŭ�� �� ���� : �÷��̾� ������ ��û */
    UFUNCTION(Server, Reliable)
    void Server_RequestPlayerData(const FString& PlayerId);

    /** ���� �� Ŭ�� : ������ ���� */
    UFUNCTION(Client, Reliable)
    void Client_ReceivePlayerData(const FPlayerCharacterData& CharData,
        const FPlayerInventoryData& InvData,
        const FPlayerSkillData& SkillData);

    /** Ŭ�� �� ���� : ������ ���� */
    UFUNCTION(Server, Reliable)
    void Server_SavePlayerData(const FPlayerCharacterData& CharData,
        const FPlayerInventoryData& InvData,
        const FPlayerSkillData& SkillData);

private:
    UPROPERTY()
    AURPNetworkManager* Owner;
};