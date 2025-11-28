#include "PathConfigLoader.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

bool FPathConfigLoader::Load(const FString& FilePath, FGameDataPacket& OutPacket)
{
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("[PathConfigLoader] Failed: %s"), *FilePath);
        return false;
    }


    TSharedPtr<FJsonObject> RootObj;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (!FJsonSerializer::Deserialize(Reader, RootObj) || !RootObj.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("[PathConfigLoader] Invalid JSON"));
        return false;
    }

    // 패킷 이름 설정
    OutPacket.TableName = TEXT("PathConfig");

    FURPPathConfig& OutConfig = OutPacket.PathConfigs;

    // Monster Config
    if (const TSharedPtr<FJsonObject>* MonsterObj;
        RootObj->TryGetObjectField(TEXT("Monster"), MonsterObj))
    {
        (*MonsterObj)->TryGetStringField(TEXT("BPBase"), OutConfig.MonsterBPBase);
        (*MonsterObj)->TryGetStringField(TEXT("DefaultBPName"), OutConfig.DefaultBPName);
        (*MonsterObj)->TryGetStringField(TEXT("MeshBase"), OutConfig.MonsterMeshBase);
        (*MonsterObj)->TryGetStringField(TEXT("AnimBP"), OutConfig.DefaultMonsterAnimBP);
        (*MonsterObj)->TryGetStringField(TEXT("DefaultBT"), OutConfig.DefaultMonsterBT);
        (*MonsterObj)->TryGetStringField(TEXT("DefaultDeath"), OutConfig.DefaultMonsterDeath);
    }

    // Player Config
    if (const TSharedPtr<FJsonObject>* PlayerObj;
        RootObj->TryGetObjectField(TEXT("Player"), PlayerObj))
    {
        (*PlayerObj)->TryGetStringField(TEXT("BPBase"), OutConfig.PlayerBPBase);
        (*PlayerObj)->TryGetStringField(TEXT("MeshBase"), OutConfig.PlayerMeshBase);
        (*PlayerObj)->TryGetStringField(TEXT("AnimBP"), OutConfig.DefaultPlayerAnimBP);

        if (const TSharedPtr<FJsonObject>* ClassObj;
            (*PlayerObj)->TryGetObjectField(TEXT("Classes"), ClassObj))
        {
            for (const auto& Pair : (*ClassObj)->Values)
            {
                OutConfig.PlayerClassMap.Add(Pair.Key, Pair.Value->AsString());
            }
        }
    }

    if (const TSharedPtr<FJsonObject>* InputObj;
        RootObj->TryGetObjectField(TEXT("Input"), InputObj))
    {
        (*InputObj)->TryGetStringField(TEXT("IMC"), OutConfig.IMC);
        (*InputObj)->TryGetStringField(TEXT("LeftClick"), OutConfig.LeftClick);
        (*InputObj)->TryGetStringField(TEXT("RightClick"), OutConfig.RightClick);
        (*InputObj)->TryGetStringField(TEXT("SkillQ"), OutConfig.SkillQ);
        (*InputObj)->TryGetStringField(TEXT("SkillW"), OutConfig.SkillW);
        (*InputObj)->TryGetStringField(TEXT("SkillE"), OutConfig.SkillE);
        (*InputObj)->TryGetStringField(TEXT("SkillR"), OutConfig.SkillR);
    }

    UE_LOG(LogTemp, Log, TEXT("[PathConfigLoader] Loaded PathConfig.json successfully"));

    return true;
}
