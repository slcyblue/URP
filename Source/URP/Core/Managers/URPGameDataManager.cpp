
#include "URPGameDataManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Kismet/GameplayStatics.h"
#include "Data/URPGameData.h"
#include "Core/Subsystems/URPNetworkSubsystem.h"

UURPGameDataManager* UURPGameDataManager::Instance = nullptr;

UURPGameDataManager* UURPGameDataManager::Get()
{
    if (!Instance) Instance = NewObject<UURPGameDataManager>();
    return Instance;
}

void UURPGameDataManager::Initialize()
{
    LoadLocalFallback();
}

void UURPGameDataManager::LoadLocalFallback()
{
    FString VersionFile = FPaths::ProjectSavedDir() / TEXT("GameDataVersion.txt");
    if (FPaths::FileExists(VersionFile))
    {
        FFileHelper::LoadFileToString(CachedVersion, *VersionFile);
    }
    else
    {
        CachedVersion = TEXT("0.0.0");
    }

    UE_LOG(LogTemp, Log, TEXT("[GameDataManager] Local version: %s"), *CachedVersion);
}

void UURPGameDataManager::ApplyServerUpdate(const FString& NewVersion, const TArray<FGameDataPacket>& UpdatedTables)
{
    // 서버에서 받은 데이터로 갱신
    CachedVersion = NewVersion;
    GameTables = UpdatedTables;

    UE_LOG(LogTemp, Log, TEXT("[GameDataManager] Updated to version %s with %d tables."),
        *NewVersion, UpdatedTables.Num());
}