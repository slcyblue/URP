#include "URPGameDataFactory.h"
#include "GameData/MonsterTableLoader.h"
#include "GameData/CharacterPresetLoader.h"
#include "GameData/PathConfigLoader.h"

TMap<FString, TSharedPtr<IGameDataLoader>> UURPGameDataFactory::Loaders;

void UURPGameDataFactory::RegisterDefaults()
{
    if (Loaders.Num() > 0) return; // 중복 방지

    Loaders.Add(TEXT("MonsterTable"), MakeShared<FMonsterTableLoader>());
    Loaders.Add(TEXT("CharacterPreset"), MakeShared<FCharacterPresetLoader>());
    Loaders.Add(TEXT("PathConfig"), MakeShared<FPathConfigLoader>());
}

bool UURPGameDataFactory::BuildPacket(const FString& TableName, const FString& FilePath, FGameDataPacket& OutPacket)
{
    if (auto Loader = Loaders.Find(TableName))
    {
        return (*Loader)->Load(FilePath, OutPacket);
    }

    UE_LOG(LogTemp, Warning, TEXT("[GameDataFactory] No loader registered for %s"), *TableName);
    return false;
}
