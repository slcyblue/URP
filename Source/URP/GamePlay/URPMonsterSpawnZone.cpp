#include "URPMonsterSpawnZone.h"
#include "Components/SphereComponent.h"
#include "NavigationSystem.h"
#include "Characters/URPMonsterCharacter.h"
#include "GameFramework/Character.h"
#include <Core/Subsystems/URPGameDataSubsystem.h>

AURPMonsterSpawnZone::AURPMonsterSpawnZone()
{
    PrimaryActorTick.bCanEverTick = false;

    ZoneTrigger = CreateDefaultSubobject<USphereComponent>("ZoneTrigger");
    ZoneTrigger->InitSphereRadius(700.f);
    ZoneTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    ZoneTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    ZoneTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    RootComponent = ZoneTrigger;
}

void AURPMonsterSpawnZone::BeginPlay()
{
    Super::BeginPlay();

    MonsterPool = NewObject<UURPMonsterPool>(this);
    MonsterPool->InitializePool(GetWorld(), MonsterClass, MaxCount);

    ZoneTrigger->OnComponentBeginOverlap.AddDynamic(this, &AURPMonsterSpawnZone::OnPlayerEnterZone);
    ZoneTrigger->OnComponentEndOverlap.AddDynamic(this, &AURPMonsterSpawnZone::OnPlayerExitZone);
}

/* 플레이어 진입 → 활성화 */
void AURPMonsterSpawnZone::OnPlayerEnterZone(
    UPrimitiveComponent* Comp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 BodyIndex,
    bool bSweep, const FHitResult& Hit)
{
    if (OtherActor->IsA(ACharacter::StaticClass()))
    {
        ActivateZone();
    }
}

/* 플레이어 퇴장 → 비활성화 */
void AURPMonsterSpawnZone::OnPlayerExitZone(
    UPrimitiveComponent* Comp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 BodyIndex)
{
    if (OtherActor->IsA(ACharacter::StaticClass()))
    {
        DeactivateZone();
    }
}

/* 활성화 */
void AURPMonsterSpawnZone::ActivateZone()
{
    if (bIsActive)
        return;

    bIsActive = true;

    // 즉시 초기 스폰
    SpawnMissingMonsters();

    // 반환 대기 타이머 취소
    GetWorldTimerManager().ClearTimer(DeactivateDelayTimer);

    // 유지 스폰 타이머
    GetWorldTimerManager().SetTimer(SpawnTimer, this,
        &AURPMonsterSpawnZone::SpawnMissingMonsters, 3.f, true);
}

/* 비활성화 */
void AURPMonsterSpawnZone::DeactivateZone()
{
    if (!bIsActive)
        return;

    bIsActive = false;

    GetWorldTimerManager().ClearTimer(SpawnTimer);

    //반환 타이머 시작
    GetWorldTimerManager().SetTimer(
        DeactivateDelayTimer,
        this,
        &AURPMonsterSpawnZone::ReturnAllMonstersToPool,
        5.0f,
        false
    );
}

/* MaxCount 유지 스폰 */
void AURPMonsterSpawnZone::SpawnMissingMonsters()
{
    if (!bIsActive) return;

    //비활성화 되어있는 몬스터들을 목록에서 제거
    CurrentMonsters.RemoveAll([](AURPMonsterCharacter* M)
        {
            return !IsValid(M) || !M->IsActive();
        });

    int Missing = MaxCount - CurrentMonsters.Num();
    if (Missing <= 0) return;

    for (int i = 0; i < Missing; i++)
    {
        FVector Loc;
        if (!FindValidSpawnPoint(Loc)) continue;

        AURPMonsterCharacter* Mob = MonsterPool->GetPooledMonster();
        if (!Mob) continue;

        Mob->SetActorLocation(Loc);

        const FURPMonsterRow* Row = GetRandomMonsterRow();
        if (!Row) continue;

        // 몬스터 데이터 초기화
        Mob->InitializeFromMonsterData(*Row, ZoneLevel);
        Mob->OwningZone = this;

        CurrentMonsters.Add(Mob);
    }
}

/* 적절한 스폰 위치 찾기 */
bool AURPMonsterSpawnZone::FindValidSpawnPoint(FVector& OutLocation)
{
    UNavigationSystemV1* Nav = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!Nav) return false;

    for (int i = 0; i < 20; i++)
    {
        FNavLocation P;
        if (Nav->GetRandomPointInNavigableRadius(GetActorLocation(), SpawnRadius, P))
        {
            FCollisionShape Capsule = FCollisionShape::MakeCapsule(40.f, 80.f);
            bool Blocked = GetWorld()->OverlapBlockingTestByChannel(
                P.Location, FQuat::Identity, ECC_Pawn, Capsule);

            if (!Blocked)
            {
                OutLocation = P.Location;
                return true;
            }
        }
    }
    return false;
}

const FURPMonsterRow* AURPMonsterSpawnZone::GetRandomMonsterRow() const
{
    if (MonsterName.Num() == 0)
        return nullptr;

    // 1) 이름 리스트에서 랜덤 선택
    FString SelectedName = MonsterName[FMath::RandRange(0, MonsterName.Num() - 1)];

    // 2) GameDataManager에서 Row 데이터 찾기
    auto* GameData = GetGameInstance()->GetSubsystem<UURPGameDataSubsystem>();
    if (!GameData)
        return nullptr;

    return GameData->GetMonsterRow(SelectedName);
}

void AURPMonsterSpawnZone::ReturnMonsterToPool(AURPMonsterCharacter* Mob)
{
    if (MonsterPool)
        MonsterPool->ReturnMonster(Mob);

    // 리스트에서 제거
    CurrentMonsters.Remove(Mob);
}

void AURPMonsterSpawnZone::ReturnAllMonstersToPool()
{
    // 플레이어가 재진입해서 Zone이 Active 상태면 반환 취소
    if (bIsActive)
        return;

    for (AURPMonsterCharacter* Mob : CurrentMonsters)
    {
        if (IsValid(Mob) && MonsterPool)
        {
            MonsterPool->ReturnMonster(Mob);
        }
    }

    CurrentMonsters.Empty();
}