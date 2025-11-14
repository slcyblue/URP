#include "URPMonsterPool.h"
#include "Characters/URPMonsterCharacter.h"

void UURPMonsterPool::InitializePool(UWorld* World, TSubclassOf<AURPMonsterCharacter> InMonsterClass, int32 MaxSize)
{
    CachedWorld = World;
    MonsterClass = InMonsterClass;
    MaxPoolSize = MaxSize;

    Pool.Reserve(MaxPoolSize);

    for (int i = 0; i < MaxPoolSize; i++)
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AURPMonsterCharacter* Mob = World->SpawnActor<AURPMonsterCharacter>(MonsterClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);

        Mob->SetActorHiddenInGame(true);
        Mob->SetActorEnableCollision(false);
        Mob->SetActive(false);

        Pool.Add(Mob);
    }
}

AURPMonsterCharacter* UURPMonsterPool::GetPooledMonster()
{
    // 1) 풀에 몹이 있으면 뽑아서 사용
    if (Pool.Num() > 0)
    {
        AURPMonsterCharacter* Mob = Pool.Pop();

        Mob->SetActorHiddenInGame(false);
        Mob->SetActorEnableCollision(true);
        Mob->SetActive(true);

        return Mob;
    }

    // 2) 풀에 몹이 없으면 새 몹 생성해서 반환 (자동 확장)
    UE_LOG(LogTemp, Warning, TEXT("[MonsterPool] Pool is empty! Spawning new monster (Dynamic Expansion)"));

    if (CachedWorld && MonsterClass)
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AURPMonsterCharacter* NewMob = CachedWorld->SpawnActor<AURPMonsterCharacter>(
            MonsterClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);

        if (NewMob)
        {
            NewMob->SetActorHiddenInGame(false);
            NewMob->SetActorEnableCollision(true);
            NewMob->SetActive(true);
            return NewMob;
        }
    }

    return nullptr;
}

void UURPMonsterPool::ReturnMonster(AURPMonsterCharacter* Monster)
{
    if (!Monster) return;

    Monster->SetActive(false);

    Monster->SetActorLocation(FVector(0, 0, -100000)); // 안전한 컬링 위치

    Pool.Add(Monster);
}
