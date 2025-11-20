#include "URPPlayerSkillComponent.h"
#include "Skill/URPSkillBase.h"
#include "Skill/URPAOEBase.h"
#include "Skill/URPDashBase.h"
#include "Skill/URPProjectileBase.h"
#include "Skill/URPSingleHitBase.h"
#include "URPPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Core/Subsystems/URPGameDataSubsystem.h"

UURPPlayerSkillComponent::UURPPlayerSkillComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UURPPlayerSkillComponent::BeginPlay()
{
    Super::BeginPlay();
}



void UURPPlayerSkillComponent::InitializeSkills(EURPClassType ClassType)
{
    AURPPlayerCharacter* OwnerPC = Cast<AURPPlayerCharacter>(GetOwner());
    if (!OwnerPC)
    {
        UE_LOG(LogTemp, Error, TEXT("SkillComponent: OwnerPC is NULL"));
        return;
    }

    UURPGameDataSubsystem* GameData = OwnerPC->GetGameInstance()->GetSubsystem<UURPGameDataSubsystem>();
    if (!GameData)
    {
        UE_LOG(LogTemp, Error, TEXT("SkillComponent: GameDataSubsystem not found"));
        return;
    }

    const TArray<FURPSkillRow>& AllRows = GameData->GetSkillTable();
    if (AllRows.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("SkillComponent: SkillTable is empty"));
        return;
    }

    for (const FURPSkillRow& Row : AllRows)
    {
        // 직업 필터
        if (Row.RequiredClass != ClassType)
            continue;

        if (!Row.SkillClass)
        {
            UE_LOG(LogTemp, Warning, TEXT("Skill Row %s has no SkillClass"), *Row.SkillName);
            continue;
        }

        UURPSkillBase* Skill = NewObject<UURPSkillBase>(this, Row.SkillClass);
        if (!Skill) continue;

        Skill->SkillId = Row.SkillId;
        Skill->Cooldown = Row.Cooldown;

        // 유형별 세팅
        switch (Row.SkillType)
        {
        case EURPSkillType::Projectile:
            if (auto* Proj = Cast<UURPProjectileBase>(Skill))
            {
                Proj->ProjectileClass = Row.ProjectileClass;
                Proj->DamageMultiplier = Row.DamageMultiplier;
                Proj->SpawnOffset = Row.SpawnOffset;
            }
            break;

        case EURPSkillType::SingleHit:
            if (auto* Single = Cast<UURPSingleHitBase>(Skill))
            {
                Single->Range = Row.Range;
                Single->Radius = Row.Radius;
                Single->DamageMultiplier = Row.DamageMultiplier;
            }
            break;

        case EURPSkillType::AOE:
            if (auto* AOE = Cast<UURPAOEBase>(Skill))
            {
                AOE->Radius = Row.Radius;
                AOE->DamageMultiplier = Row.DamageMultiplier;
            }
            break;

        case EURPSkillType::Dash:
            if (auto* Dash = Cast<UURPDashBase>(Skill))
            {
                Dash->DashDistance = Row.DashDistance;
                Dash->HitRadius = Row.HitRadius;
                Dash->DamageMultiplier = Row.DamageMultiplier;
            }
            break;
        }

        RegisterSkill(Skill->SkillId, Skill);

        UE_LOG(LogTemp, Log, TEXT("Loaded Skill from GameDataSubsystem: %s (Id=%d)"), *Row.SkillName, Row.SkillId);
    }
}

void UURPPlayerSkillComponent::RegisterSkill(int32 SkillId, UURPSkillBase* Skill)
{
    if (Skill)
    {
        Skill->SkillId = SkillId;
        SkillMap.Add(SkillId, Skill);
    }
}

UURPSkillBase* UURPPlayerSkillComponent::GetSkill(int32 SkillId) const
{
    if (SkillMap.Contains(SkillId))
        return SkillMap[SkillId];
    return nullptr;
}

void UURPPlayerSkillComponent::ExecuteSkill(int32 SkillId)
{
    UURPSkillBase* Skill = GetSkill(SkillId);
    if (!Skill) return;

    AURPPlayerCharacter* Owner = Cast<AURPPlayerCharacter>(GetOwner());
    if (!Owner || !Owner->HasAuthority()) return;

    float ServerTime = Owner->GetWorld()->GetTimeSeconds();
    if (!Skill->IsReady(ServerTime))
        return;

    Skill->Execute(Owner);
    Skill->StartCooldown(ServerTime);
}

float UURPPlayerSkillComponent::GetRemainingCooldown(int32 SkillId) const
{
    if (auto* Skill = GetSkill(SkillId))
    {
        float Now = GetWorld()->GetTimeSeconds();
        return FMath::Max(0.f, Skill->NextUseTime - Now);
    }
    return 0.f;
}

float UURPPlayerSkillComponent::GetCooldown(int32 SkillId) const
{
    if (auto* Skill = GetSkill(SkillId))
    {
        return Skill->Cooldown;
    }
    return 0.f;
}