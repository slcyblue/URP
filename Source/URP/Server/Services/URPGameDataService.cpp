#include "Server/Services/URPGameDataService.h"
#include "Server/Storage/URPGameDataFactory.h"

UURPGameDataService::UURPGameDataService()
{
    RegisterHandler<UURPGameDataService>(EURPServerRequestType::SyncGameData, &UURPGameDataService::OnSyncGameData);
}

bool UURPGameDataService::OnSyncGameData(const void* Payload, void* OutResponse)
{
    const FGameDataSyncRequest* Req = static_cast<const FGameDataSyncRequest*>(Payload);
    FGameDataSyncResponse* Res = static_cast<FGameDataSyncResponse*>(OutResponse);
    if (!Req || !Res) return false;

    const bool bOK = BuildSyncResponse(*Req, *Res);

    UE_LOG(LogTemp, Log, TEXT("[GameDataService] Sync request client=%s -> %s (UpToDate=%s)"),
        *Req->ClientVersion,
        bOK ? TEXT("RESPONDED") : TEXT("FAIL"),
        Res->bUpToDate ? TEXT("True") : TEXT("False"));

    return bOK;
}

bool UURPGameDataService::BuildSyncResponse(const FGameDataSyncRequest& Req, FGameDataSyncResponse& Out)
{
    Out.NewVersion = TEXT("1.0.1");
    Out.bUpToDate = false;

    // 팩토리 등록
    UURPGameDataFactory::RegisterDefaults();

    const FString BasePath = FPaths::ProjectContentDir() / TEXT("ServerData");


    // 로드할 Json 테이블 이름 추가
    TArray<FString> Tables = {
        TEXT("MonsterTable"),
        TEXT("CharacterPreset"),
        TEXT("PathConfig")
    };

    // 패킷 생성
    for (const FString& TableName : Tables)
    {
        FString FilePath = BasePath / (TableName + TEXT(".json"));
        FGameDataPacket Packet;
        if (UURPGameDataFactory::BuildPacket(TableName, FilePath, Packet))
        {
            Out.UpdatedTables.Add(Packet);
            UE_LOG(LogTemp, Log, TEXT("[GameDataService] Loaded %s (%d entries)"), *TableName, Out.UpdatedTables.Num());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[GameDataService] Failed to load %s"), *TableName);
        }
    }

    return true;
}