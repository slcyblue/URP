// Fill out your copyright notice in the Description page of Project Settings.

#include "URPGameDataManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

UURPGameDataManager* UURPGameDataManager::Get(UWorld* World)
{
    static UURPGameDataManager* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UURPGameDataManager>();
        Instance->Initialize(World);
    }
    return Instance;
}

void UURPGameDataManager::Initialize(UWorld* World)
{
    CachedJson.Empty();

    if (World && World->GetAuthGameMode()) // 서버 권한이면
    {
        LoadAllFromServerDisk();
    }
    else // 클라이언트
    {
        LoadLocalCache();
    }
}

/** 서버 전용: /Content/Data 폴더의 JSON 전체 로드 */
void UURPGameDataManager::LoadAllFromServerDisk()
{
    const FString BasePath = FPaths::ProjectContentDir() / TEXT("Data/");
    TArray<FString> FileList;
    IFileManager::Get().FindFiles(FileList, *BasePath, TEXT("json"));

    for (const FString& File : FileList)
    {
        FString Json;
        if (FFileHelper::LoadFileToString(Json, *(BasePath + File)))
        {
            FString Key = FPaths::GetBaseFilename(File);
            CachedJson.Add(Key, Json);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("[GameDataManager] Loaded %d JSON files from disk."), CachedJson.Num());
}

/** 모든 데이터 → 네트워크 전송용 패킷 배열로 변환 */
void UURPGameDataManager::GetAllDataPackets(TArray<FGameDataPacket>& OutPackets) const
{
    for (const auto& Pair : CachedJson)
    {
        FGameDataPacket Packet;
        Packet.DataName = Pair.Key;
        Packet.JsonString = Pair.Value;
        OutPackets.Add(Packet);
    }
}

/** 클라이언트 캐시 로드 */
void UURPGameDataManager::LoadLocalCache()
{
    const FString CachePath = FPaths::ProjectSavedDir() / TEXT("CachedData.json");
    FString JsonString;

    if (FFileHelper::LoadFileToString(JsonString, *CachePath))
    {
        TSharedPtr<FJsonObject> Root;
        auto Reader = TJsonReaderFactory<>::Create(JsonString);

        if (FJsonSerializer::Deserialize(Reader, Root))
        {
            for (auto& Pair : Root->Values)
            {
                CachedJson.Add(Pair.Key, Pair.Value->AsString());
            }

            UE_LOG(LogTemp, Log, TEXT("[GameDataManager] Loaded %d cached entries."), CachedJson.Num());
        }
    }
}

/** 클라이언트 캐시 저장 */
void UURPGameDataManager::SaveLocalCache()
{
    TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();
    for (auto& Pair : CachedJson)
    {
        Root->SetStringField(Pair.Key, Pair.Value);
    }

    FString Output;
    auto Writer = TJsonWriterFactory<>::Create(&Output);
    FJsonSerializer::Serialize(Root.ToSharedRef(), Writer);

    const FString CachePath = FPaths::ProjectSavedDir() / TEXT("CachedData.json");
    FFileHelper::SaveStringToFile(Output, *CachePath);

    UE_LOG(LogTemp, Log, TEXT("[GameDataManager] Saved local cache (%d entries)."), CachedJson.Num());
}

/** 데이터 적용 (서버·클라 공용) */
void UURPGameDataManager::ApplyData(const FString& DataName, const FString& JsonString)
{
    CachedJson.Add(DataName, JsonString);
}

/** 특정 데이터 반환 */
FString UURPGameDataManager::GetDataAsString(FString DataName) const
{
    const FString* Found = CachedJson.Find(DataName);
    return Found ? *Found : TEXT("");
}

/** 요약 출력 */
void UURPGameDataManager::PrintSummary() const
{
    UE_LOG(LogTemp, Log, TEXT("[GameDataManager] %d entries loaded:"), CachedJson.Num());
    for (auto& Pair : CachedJson)
    {
        UE_LOG(LogTemp, Log, TEXT(" - %s"), *Pair.Key);
    }
}