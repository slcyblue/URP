#include "PlayerDataRPCHandler.h"
#include "../Singletons/URPNetworkManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "JsonObjectConverter.h"

void UPlayerDataRPCHandler::Init(AURPNetworkManager* InOwner)
{
    Owner = InOwner;
}

void UPlayerDataRPCHandler::Server_RequestPlayerData_Implementation(const FString& PlayerId)
{
    const FString BasePath = FPaths::ProjectSavedDir() / TEXT("ServerData/") / PlayerId;
    FPlayerCharacterData CharData;
    FPlayerInventoryData InvData;
    FPlayerSkillData SkillData;

    auto LoadData = [&](auto& Struct, const FString& FileName)
        {
            FString Json;
            if (FFileHelper::LoadFileToString(Json, *(BasePath / FileName)))
                FJsonObjectConverter::JsonObjectStringToUStruct(Json, &Struct);
            Struct.PlayerId = PlayerId;
        };

    LoadData(CharData, TEXT("Character.json"));
    LoadData(InvData, TEXT("Inventory.json"));
    LoadData(SkillData, TEXT("Skill.json"));

    Client_ReceivePlayerData(CharData, InvData, SkillData);
}

void UPlayerDataRPCHandler::Client_ReceivePlayerData_Implementation(
    const FPlayerCharacterData& CharData,
    const FPlayerInventoryData& InvData,
    const FPlayerSkillData& SkillData)
{
    UE_LOG(LogTemp, Log, TEXT("[Client] Received PlayerData for %s (Lv.%d)"), *CharData.PlayerId, CharData.Level);

    // TODO: GameSaveManager 또는 PlayerState에 반영
}

void UPlayerDataRPCHandler::Server_SavePlayerData_Implementation(
    const FPlayerCharacterData& CharData,
    const FPlayerInventoryData& InvData,
    const FPlayerSkillData& SkillData)
{
    if (!Owner)
        return;

    FString AuthId = Owner->GetAuthenticatedPlayerId();  // NetworkManager에서 가져옴

    if (AuthId != CharData.PlayerId)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Server] Unauthorized save attempt: %s"), *CharData.PlayerId);
        return;
    }

    // 정상 저장 로직
    //SaveData(CharData, InvData, SkillData);
}