#include "SkillDataLoader.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

bool FSkillDataLoader::Load(const FString& FilePath, FGameDataPacket& OutPacket)
{
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("[SkillDataLoader] File not found: %s"), *FilePath);
        return false;
    }

    TArray<FURPSkillRow> Rows;
    if (!FJsonObjectConverter::JsonArrayStringToUStruct(JsonString, &Rows, 0, 0))
    {
        UE_LOG(LogTemp, Error, TEXT("[SkillDataLoader] Failed to parse JSON: %s"), *FilePath);
        return false;
    }

    OutPacket.SkillDatas = Rows;
    return true;
}
