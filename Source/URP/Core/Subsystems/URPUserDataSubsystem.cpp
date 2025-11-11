
#include "URPUserDataSubsystem.h"


void UURPUserDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Log, TEXT("[UserDataManager] Subsystem Initialized"));
}

void UURPUserDataSubsystem::SetUserData(const FPlayerData& InData)
{
    CachedPlayerData = InData;

    UE_LOG(LogTemp, Log, TEXT("[UserDataManager] PlayerData cached -> Name: %s | Level: %d | Gold: %d"),
        *CachedPlayerData.PlayerId, CachedPlayerData.Level, CachedPlayerData.Gold);
}