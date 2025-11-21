#include "ClassDataLoader.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

bool FClassDataLoader::Load(const FString& FilePath, FGameDataPacket& OutPacket)
{
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("[ClassDataLoader] File not found: %s"), *FilePath);
        return false;
    }

    TArray<FURPClassData> Rows;
    if (!FJsonObjectConverter::JsonArrayStringToUStruct(JsonString, &Rows, 0, 0))
    {
        UE_LOG(LogTemp, Error, TEXT("[ClassDataLoader] Failed to parse JSON: %s"), *FilePath);
        return false;
    }

    OutPacket.TableName = TEXT("ClassData");
    OutPacket.ClassData = Rows;
    return true;
}
