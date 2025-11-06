
#include "URPUserDataManager.h"

UURPUserDataManager* UURPUserDataManager::Instance = nullptr;

UURPUserDataManager* UURPUserDataManager::Get()
{
    if (!Instance) Instance = NewObject<UURPUserDataManager>();
    return Instance;
}

void UURPUserDataManager::SetUserData(const FPlayerData& InData)
{
    CachedPlayerData = InData;

    UE_LOG(LogTemp, Log, TEXT("[UserDataManager] PlayerData cached -> Name: %s | Level: %d | Gold: %d"),
        *CachedPlayerData.PlayerId, CachedPlayerData.Level, CachedPlayerData.Gold);
}