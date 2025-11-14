#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "URPMonsterPool.generated.h"

class AURPMonsterCharacter;

UCLASS()
class URP_API UURPMonsterPool : public UObject
{
    GENERATED_BODY()

public:
    void InitializePool(UWorld* World, TSubclassOf<AURPMonsterCharacter> MonsterClass, int32 MaxSize);

    AURPMonsterCharacter* GetPooledMonster();
    void ReturnMonster(AURPMonsterCharacter* Monster);

private:
    UPROPERTY()
    TArray<AURPMonsterCharacter*> Pool;

    UPROPERTY()
    UWorld* CachedWorld;

    UPROPERTY()
    TSubclassOf<AURPMonsterCharacter> MonsterClass;

    int32 MaxPoolSize = 0;
};
