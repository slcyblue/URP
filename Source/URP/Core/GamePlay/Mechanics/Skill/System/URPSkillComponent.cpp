#include "URPSkillComponent.h"
#include "Core/GamePlay/Mechanics/Skill/Base/URPSkillBase.h"
#include "Core/GamePlay/Mechanics/Skill/Attack/URPAOEBase.h"
#include "Core/GamePlay/Mechanics/Skill/Attack/URPDashBase.h"
#include "Core/GamePlay/Mechanics/Skill/Attack/URPProjectileBase.h"
#include "Core/GamePlay/Mechanics/Skill/Attack/URPSingleHitBase.h"
#include "Characters/Player/URPPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Core/Subsystems/Data/URPGameDataSubsystem.h"

UURPSkillComponent::UURPSkillComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UURPSkillComponent::BeginPlay()
{
    Super::BeginPlay();
}



void UURPSkillComponent::InitializeSkills(EURPClassType ClassType)
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

        UURPSkillBase* Skill = nullptr;

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

        if (!Skill)
            continue;

        Skill->SkillId = Row.SkillId;
        Skill->Cooldown = Row.Cooldown;

        RegisterSkill(Skill->SkillId, Skill);

        UE_LOG(LogTemp, Log, TEXT("Loaded Skill from GameDataSubsystem: %s (Id=%d)"), *Row.SkillName, Row.SkillId);
    }
}

void UURPSkillComponent::RegisterSkill(int32 SkillId, UURPSkillBase* Skill)
{
    if (Skill)
    {
        Skill->SkillId = SkillId;
        SkillMap.Add(SkillId, Skill);
    }
}

UURPSkillBase* UURPSkillComponent::GetSkill(int32 SkillId) const
{
    if (SkillMap.Contains(SkillId))
        return SkillMap[SkillId];
    return nullptr;
}

void UURPSkillComponent::ExecuteSkill(int32 SkillId, float AdjustTime)
{
    UURPSkillBase* Skill = SkillMap.FindRef(SkillId);
    if (!Skill) 
        return;

    AURPCharacterBase* Owner = Cast<AURPCharacterBase>(GetOwner());
    if (!Owner) return;

    // 쿨다운 체크 → Time 기준
    if (!Skill->IsReady(AdjustTime))
        return;

    Skill->Execute(Owner);
    Skill->StartCooldown(AdjustTime);
}

float UURPSkillComponent::GetRemainingCooldown(int32 SkillId) const
{
    if (auto* Skill = GetSkill(SkillId))
    {
        float Now = GetWorld()->GetTimeSeconds();
        return FMath::Max(0.f, Skill->NextUseTime - Now);
    }
    return 0.f;
}

float UURPSkillComponent::GetCooldown(int32 SkillId) const
{
    if (auto* Skill = GetSkill(SkillId))
    {
        return Skill->Cooldown;
    }
    return 0.f;
}