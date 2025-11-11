#include "CharacterPresetLoader.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

bool FCharacterPresetLoader::Load(const FString& FilePath, FGameDataPacket& OutPacket)
{
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("[CharacterPresetLoader] File not found: %s"), *FilePath);
        return false;
    }

    FCharacterPresetTable PresetTable;
    if (!FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &PresetTable, 0, 0))
    {
        UE_LOG(LogTemp, Error, TEXT("[CharacterPresetLoader] Failed to parse JSON: %s"), *FilePath);
        return false;
    }

    OutPacket.TableName = TEXT("CharacterPreset");
    OutPacket.CharacterPresets = PresetTable;
    return true;
}
