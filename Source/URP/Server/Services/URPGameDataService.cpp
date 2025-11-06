#include "Server/Services/URPGameDataService.h"

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
    // TODO: 실제 서버 버전 비교 로직
    //Out.bUpToDate = false;
    //Out.NewVersion = TEXT("1.2.3");
    //Out.UpdatedTables = { TEXT("MonsterTable"), TEXT("ItemTable"), TEXT("QuestTable") };

    return true;
}