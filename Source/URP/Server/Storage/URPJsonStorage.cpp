#include "Server/Storage/URPJsonStorage.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "JsonObjectConverter.h"

bool UURPJsonStorage::SaveStruct(const FString& Category,
    const FString& Key,
    const void* StructData,
    const UStruct* StructType)
{
    if (!StructType || !StructData)
    {
        UE_LOG(LogTemp, Error, TEXT("[URPJsonStorage] SaveStruct: Invalid StructType or Data."));
        return false;
    }

    if (!EnsureCategoryDir(Category))
    {
        UE_LOG(LogTemp, Error, TEXT("[URPJsonStorage] SaveStruct: Directory creation failed (%s)."), *Category);
        return false;
    }

    FString OutJson;
    if (!FJsonObjectConverter::UStructToJsonObjectString(StructType, StructData, OutJson, 0, 0))
    {
        UE_LOG(LogTemp, Error, TEXT("[URPJsonStorage] Failed to serialize struct: %s"), *StructType->GetName());
        return false;
    }

    const FString FullPath = GetFullPath(Category, Key);
    if (!FFileHelper::SaveStringToFile(OutJson, *FullPath))
    {
        UE_LOG(LogTemp, Error, TEXT("[URPJsonStorage] SaveStruct: Failed to write file: %s"), *FullPath);
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("[URPJsonStorage] Saved struct: %s"), *FullPath);
    return true;
}

bool UURPJsonStorage::LoadStruct(const FString& Category,
    const FString& Key,
    void* OutStruct,
    const UStruct* StructType)
{
    if (!StructType || !OutStruct)
    {
        UE_LOG(LogTemp, Error, TEXT("[URPJsonStorage] LoadStruct: Invalid StructType or OutStruct."));
        return false;
    }

    const FString FullPath = GetFullPath(Category, Key);
    if (!FPaths::FileExists(FullPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("[URPJsonStorage] LoadStruct: File not found: %s"), *FullPath);
        return false;
    }

    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FullPath))
    {
        UE_LOG(LogTemp, Error, TEXT("[URPJsonStorage] LoadStruct: Failed to read file: %s"), *FullPath);
        return false;
    }

    //  최신 UE 방식 (Json → FJsonObject → Struct)
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("[URPJsonStorage] LoadStruct: JSON parse failed for: %s"), *FullPath);
        return false;
    }

    if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), StructType, OutStruct, 0, 0))
    {
        UE_LOG(LogTemp, Error, TEXT("[URPJsonStorage] LoadStruct: Struct conversion failed (%s)"), *StructType->GetName());
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("[URPJsonStorage] Loaded struct: %s"), *FullPath);
    return true;
}

bool UURPJsonStorage::Exists(const FString& Category, const FString& Key) const
{
    return FPaths::FileExists(GetFullPath(Category, Key));
}

FString UURPJsonStorage::GetFullPath(const FString& Category, const FString& Key) const
{
    return FPaths::ProjectSavedDir() / TEXT("ServerData") / Category / (Key + TEXT(".json"));
}

bool UURPJsonStorage::EnsureCategoryDir(const FString& Category) const
{
    IPlatformFile& PF = FPlatformFileManager::Get().GetPlatformFile();
    const FString Dir = FPaths::ProjectSavedDir() / TEXT("ServerData") / Category;
    return PF.CreateDirectoryTree(*Dir);
}
