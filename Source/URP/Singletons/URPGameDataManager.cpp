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

    if (World && World->GetAuthGameMode()) // ���� �����̸�
    {
        LoadAllFromServerDisk();
    }
    else // Ŭ���̾�Ʈ
    {
        LoadLocalCache();
    }
}

/** ���� ����: /Content/Data ������ JSON ��ü �ε� */
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

/** ��� ������ �� ��Ʈ��ũ ���ۿ� ��Ŷ �迭�� ��ȯ */
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

/** Ŭ���̾�Ʈ ĳ�� �ε� */
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

/** Ŭ���̾�Ʈ ĳ�� ���� */
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

/** ������ ���� (������Ŭ�� ����) */
void UURPGameDataManager::ApplyData(const FString& DataName, const FString& JsonString)
{
    CachedJson.Add(DataName, JsonString);
}

/** Ư�� ������ ��ȯ */
FString UURPGameDataManager::GetDataAsString(FString DataName) const
{
    const FString* Found = CachedJson.Find(DataName);
    return Found ? *Found : TEXT("");
}

/** ��� ��� */
void UURPGameDataManager::PrintSummary() const
{
    UE_LOG(LogTemp, Log, TEXT("[GameDataManager] %d entries loaded:"), CachedJson.Num());
    for (auto& Pair : CachedJson)
    {
        UE_LOG(LogTemp, Log, TEXT(" - %s"), *Pair.Key);
    }
}