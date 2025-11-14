
#include "URPGameDataSubsystem.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Kismet/GameplayStatics.h"
#include "Data/URPGameData.h"
#include "Core/Subsystems/URPNetworkSubsystem.h"
#include "Network/GameDataRPCHandler.h"

void UURPGameDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(LogTemp, Log, TEXT("[GameDataManager] Subsystem Initialized"));

    Collection.InitializeDependency<UURPNetworkSubsystem>();

    LoadLocalFallback();

    InitializeGameData();
}

void UURPGameDataSubsystem::InitializeGameData()
{
    UGameInstance* GameInstance = GetGameInstance();
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("[GameDataManager] GameInstance is null."));
        return;
    }

    // 네트워크 서브시스템 가져오기
    if (UURPNetworkSubsystem* Network = GameInstance->GetSubsystem<UURPNetworkSubsystem>())
    {
        if (auto* Handler = Network->GetHandler<UGameDataRPCHandler>())
        {
            Handler->OnGameDataSyncResponse.RemoveAll(this);
            Handler->OnGameDataSyncResponse.AddDynamic(this, &UURPGameDataSubsystem::ApplyServerUpdate);

            FGameDataSyncRequest Req;
            Req.ClientVersion = CachedVersion;

            UE_LOG(LogTemp, Log, TEXT("[GameDataManager] Requesting game data sync. Local version: %s"), *CachedVersion);
            Handler->Server_RequestGameDataSync(Req);
            return;
        }
        UE_LOG(LogTemp, Warning, TEXT("[GameDataManager] GameDataRPCHandler not found."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[GameDataManager] NetworkSubsystem not found in GameInstance."));
    }
}


void UURPGameDataSubsystem::LoadLocalFallback()
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

void UURPGameDataSubsystem::ApplyServerUpdate(const FGameDataSyncResponse& Response)
{
    // 서버에서 받은 데이터로 갱신
    CachedVersion = Response.NewVersion;
    GameTables = Response.UpdatedTables;

    UE_LOG(LogTemp, Log, TEXT("[GameDataManager] Updated to version %s with %d tables."),
        *Response.NewVersion, Response.UpdatedTables.Num());

    for (const FGameDataPacket& Packet : Response.UpdatedTables)
    {
        if (Packet.TableName == TEXT("MonsterTable"))
        {
            MonsterTable = Packet.MonsterRows;
        }
        /*else if (Packet.TableName == TEXT("ItemTable"))
        {
            ItemTable = Packet.ItemRows;
        }*/
        else if (Packet.TableName == TEXT("CharacterPreset"))
        {
            PresetTable = Packet.CharacterPresets;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("[GameDataManager] Updated to version %s with %d tables."), *Response.NewVersion, Response.UpdatedTables.Num());
}

TOptional<FString> UURPGameDataSubsystem::GetPawnPathByClass(EURPClassType ClassType) const
{
    for (const auto& P : PresetTable.Presets)
    {
        if (P.Class == ClassType && !P.PawnClassPath.IsEmpty())
            return P.PawnClassPath;
    }
    return {};
}

const FURPMonsterRow* UURPGameDataSubsystem::GetMonsterRow(const FString ID)
{
    for (const auto& MT : MonsterTable)
    {
        if (MT.Name == ID) 
        {
            return &MT;
        }
    }
    return nullptr;
}
