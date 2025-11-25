#include "URPGameDataFactory.h"
#include "GameData/MonsterTableLoader.h"
#include "GameData/PathConfigLoader.h"
#include "GameData/SkillDataLoader.h"
#include "GameData//ClassDataLoader.h"

TMap<FString, TSharedPtr<IGameDataLoader>> UURPGameDataFactory::Loaders;

void UURPGameDataFactory::RegisterDefaults()
{
    if (Loaders.Num() > 0) return; // 중복 방지

    Loaders.Add(TEXT("MonsterTable"), MakeShared<FMonsterTableLoader>());
    Loaders.Add(TEXT("PathConfig"), MakeShared<FPathConfigLoader>());
    Loaders.Add(TEXT("ClassData"), MakeShared<FClassDataLoader>());
    
    TSharedPtr<IGameDataLoader> SkillLoader = MakeShared<FSkillDataLoader>();
    Loaders.Add(TEXT("Skill_Attack"), SkillLoader);
    Loaders.Add(TEXT("Skill_Buff"), SkillLoader);
    Loaders.Add(TEXT("Skill_Debuff"), SkillLoader);
    Loaders.Add(TEXT("Skill_DOT"), SkillLoader);
}

bool UURPGameDataFactory::BuildPacket(const FString& TableName, const FString& FilePath, FGameDataPacket& OutPacket)
{
    if (auto Loader = Loaders.Find(TableName))
    {
        if ((*Loader)->Load(FilePath, OutPacket))
        {
            OutPacket.TableName = TableName;
            return true;
        }
        return false;
    }

    UE_LOG(LogTemp, Warning, TEXT("[GameDataFactory] No loader registered for %s"), *TableName);
    return false;
}
