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
    FPlayerData Existing;
    if (Storage->Exists(TEXT("Players"), Req->PlayerId) &&
        Storage->LoadStruct(TEXT("Players"), Req->PlayerId, &Existing, FPlayerData::StaticStruct()))
    {
        Res->bSuccess = true;
        Res->PlayerData = Existing;
        Res->Message = TEXT("Player already initialized.");
        UE_LOG(LogTemp, Log, TEXT("[PlayerDataService] Init skipped, already exists: %s"), *Req->PlayerId);
        return true;
    }

    FPlayerData Data;
    Data.PlayerId = Req->PlayerId;
    Data.Level = 1;
    Data.Exp = 0;
    Data.Gold = 100;
    Data.SelectedClass = (Req->PlayerData.SelectedClass != EURPClassType::None) ? Req->PlayerData.SelectedClass : EURPClassType::Barbarian;
    Data.InventoryItems = {};   // 필요시 스타터 아이템 세팅
    Data.SkillLevels = {};   // 필요시 스타터 스킬 세팅

    const bool bSaved = SavePlayerData(Data.PlayerId, Data);

    Res->bSuccess = bSaved;
    Res->PlayerData = Data;
    Res->Message = bSaved ? TEXT("Initialized new PlayerData") : TEXT("Failed to initialize PlayerData");

    UE_LOG(LogTemp, Log, TEXT("[PlayerDataService] InitPlayer(%s): %s (Class=%d)"),
        *Data.PlayerId, bSaved ? TEXT("SAVED") : TEXT("FAIL"), (uint8)Data.SelectedClass);
    return bSaved;
}


bool UURPPlayerDataService::OnLoadPlayer(const void* Payload, void* OutResponse)
{
    const FPlayerDataRequest* Req = static_cast<const FPlayerDataRequest*>(Payload);
    FPlayerDataResponse* Res = static_cast<FPlayerDataResponse*>(OutResponse);
    if (!Req || !Res) return false;

    FPlayerData OutData;
    const bool bLoaded = LoadPlayerData(Req->PlayerData.PlayerId, OutData);

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

    const bool bOK = SavePlayerData(Req->PlayerData.PlayerId, Req->PlayerData);
    Res->bSuccess = bOK;
    Res->PlayerData = Req->PlayerData;
    Res->Message = bOK ? TEXT("Player data saved successfully.") : TEXT("Failed to save player data.");

    UE_LOG(LogTemp, Log, TEXT("[PlayerDataService] SavePlayer(%s): %s"), *Req->PlayerData.PlayerId, bOK ? TEXT("OK") : TEXT("FAIL"));
    return bOK;
}

bool UURPPlayerDataService::LoadPlayerData(const FString& PlayerId, FPlayerData& OutData)
{
    const bool bExists = Storage->Exists(TEXT("Players"), PlayerId);
    if (!bExists)
    {
        UE_LOG(LogTemp, Warning, TEXT("[PlayerDataService] Player %s not found, creating default data"), *PlayerId);
        OutData.PlayerId = PlayerId;
        OutData.Level = 1;
        OutData.Gold = 100;
        OutData.SelectedClass = EURPClassType::None;
        SavePlayerData(PlayerId, OutData);
        return true;
    }

    return Storage->LoadStruct(TEXT("Players"), PlayerId, &OutData, FPlayerData::StaticStruct());
}

bool UURPPlayerDataService::SavePlayerData(const FString& PlayerId, const FPlayerData& Data)
{
    if (!Storage) return false;

    return Storage->SaveStruct(TEXT("Players"), PlayerId, &Data, FPlayerData::StaticStruct());
}