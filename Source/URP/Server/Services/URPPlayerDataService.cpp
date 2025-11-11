#include "Server/Services/URPPlayerDataService.h"

UURPPlayerDataService::UURPPlayerDataService()
{
    RegisterHandler<UURPPlayerDataService>(EURPServerRequestType::CreatePlayer, &UURPPlayerDataService::OnCreatePlayer);
    RegisterHandler<UURPPlayerDataService>(EURPServerRequestType::LoadPlayer, &UURPPlayerDataService::OnLoadPlayer);
    RegisterHandler<UURPPlayerDataService>(EURPServerRequestType::SavePlayer, &UURPPlayerDataService::OnSavePlayer);
}


bool UURPPlayerDataService::OnCreatePlayer(const void* Payload, void* OutResponse)
{
    const FPlayerDataRequest* Req = static_cast<const FPlayerDataRequest*>(Payload);
    FPlayerDataResponse* Res = static_cast<FPlayerDataResponse*>(OutResponse);
    if (!Req || !Res) return false;

    // 이미 있으면 덮어쓰지 않도록 보호(원하면 덮어쓰기 정책으로 바꿔도 됨)
    FPlayerData Data;
    
    // 기존 데이터가 없으면 새로 생성
    if (!Storage->Exists(TEXT("Players"), Req->PlayerId))
    {
        Data.PlayerId = Req->PlayerId;
        Data.Level = 1;
        Data.Exp = 0;
        Data.Gold = 100;
        Data.SelectedClass = Req->PlayerData.SelectedClass;
        Data.InventoryItems = {};
        Data.SkillLevels = {};

        SavePlayerData(Data.PlayerId, Data);
        Res->bSuccess = true;
        Res->PlayerData = Data;
        Res->Message = TEXT("Created new PlayerData.");
        return true;
    }

    // 기존 데이터가 있으면 업데이트
    Storage->LoadStruct(TEXT("Players"), Req->PlayerId, &Data, FPlayerData::StaticStruct());
    if (Req->PlayerData.SelectedClass != EURPClassType::None)
        Data.SelectedClass = Req->PlayerData.SelectedClass;

    SavePlayerData(Data.PlayerId, Data);
    Res->bSuccess = true;
    Res->PlayerData = Data;
    Res->Message = TEXT("Updated PlayerData.");
    return true;
}


bool UURPPlayerDataService::OnLoadPlayer(const void* Payload, void* OutResponse)
{
    const FPlayerDataRequest* Req = static_cast<const FPlayerDataRequest*>(Payload);
    FPlayerDataResponse* Res = static_cast<FPlayerDataResponse*>(OutResponse);
    if (!Req || !Res) return false;

    FPlayerData OutData;
    const bool bLoaded = LoadPlayerData(Req->PlayerId, OutData);

    Res->bSuccess = bLoaded;
    Res->PlayerData = OutData;
    Res->Message = bLoaded ? TEXT("Player data loaded successfully.") : TEXT("Failed to load player data.");

    UE_LOG(LogTemp, Log, TEXT("[PlayerDataService] LoadPlayerData(%s): %s"),
        *Req->PlayerData.PlayerId, bLoaded ? TEXT("OK") : TEXT("FAIL"));
    return bLoaded;
}

bool UURPPlayerDataService::OnSavePlayer(const void* Payload, void* OutResponse)
{
    const FPlayerDataRequest* Req = static_cast<const FPlayerDataRequest*>(Payload);
    FPlayerDataResponse* Res = static_cast<FPlayerDataResponse*>(OutResponse);
    if (!Req || !Res) return false;

    const bool bOK = SavePlayerData(Req->PlayerId, Req->PlayerData);
    Res->bSuccess = bOK;
    Res->PlayerData = Req->PlayerData;
    Res->Message = bOK ? TEXT("Player data saved successfully.") : TEXT("Failed to save player data.");

    UE_LOG(LogTemp, Log, TEXT("[PlayerDataService] SavePlayer(%s): %s"), *Req->PlayerId, bOK ? TEXT("OK") : TEXT("FAIL"));
    return bOK;
}

bool UURPPlayerDataService::LoadPlayerData(const FString& PlayerId, FPlayerData& OutData)
{
    // 파일 없으면 생성하지 않음
    if (!Storage->Exists(TEXT("Players"), PlayerId))
    {
        return false;
    }

    if (!Storage->LoadStruct(TEXT("Players"), PlayerId, &OutData, FPlayerData::StaticStruct()))
        return false;

    return true;
}

bool UURPPlayerDataService::SavePlayerData(const FString& PlayerId, const FPlayerData& Data)
{
    if (!Storage) return false;

    return Storage->SaveStruct(TEXT("Players"), PlayerId, &Data, FPlayerData::StaticStruct());
}