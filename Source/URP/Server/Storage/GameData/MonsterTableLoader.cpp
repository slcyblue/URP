#include "MonsterTableLoader.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

bool FMonsterTableLoader::Load(const FString& FilePath, FGameDataPacket& OutPacket)
{
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("[MonsterTableLoader] File not found: %s"), *FilePath);
        return false;
    }

    TArray<FURPMonsterRow> Rows;
    if (!FJsonObjectConverter::JsonArrayStringToUStruct(JsonString, &Rows, 0, 0))
    {
        UE_LOG(LogTemp, Error, TEXT("[MonsterTableLoader] Failed to parse JSON: %s"), *FilePath);
        return false;
    }

    OutPacket.TableName = TEXT("MonsterTable");
    OutPacket.MonsterRows = Rows;

    return true;
}
