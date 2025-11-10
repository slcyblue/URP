#include "GameDataFactory.h"
#include "MonsterTableLoader.h"
#include "CharacterPresetLoader.h"

TMap<FString, TSharedPtr<IGameDataLoader>> UGameDataFactory::Loaders;

void UGameDataFactory::RegisterDefaults()
{
    if (Loaders.Num() > 0) return;
    Loaders.Add(TEXT("MonsterTable"), MakeShared<FMonsterTableLoader>());
    Loaders.Add(TEXT("CharacterPreset"), MakeShared<FCharacterPresetLoader>());
}

bool UGameDataFactory::BuildPacket(const FString& TableName, const FString& FilePath, FGameDataPacket& OutPacket)
{
    if (auto Loader = Loaders.Find(TableName))
    {
        return (*Loader)->Load(FilePath, OutPacket);
    }
    UE_LOG(LogTemp, Warning, TEXT("[GameDataFactory] No loader registered for %s"), *TableName);
    return false;
}
