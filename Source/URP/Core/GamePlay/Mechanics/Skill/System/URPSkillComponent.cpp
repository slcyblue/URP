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
    auto* OwnerChar = Cast<AURPCharacterBase>(GetOwner());
    if (!OwnerChar)
    {
        UE_LOG(LogTemp, Error, TEXT("SkillComponent: Owner is NULL"));
        return;
    }

    UURPGameDataSubsystem* GameData =
        GetWorld()->GetGameInstance()->GetSubsystem<UURPGameDataSubsystem>();
    if (!GameData)
    {
        UE_LOG(LogTemp, Error, TEXT("SkillComponent: GameDataSubsystem not found"));
        return;
    }

    const TArray<FURPSkillRow>& AllRows = GameData->GetSkillTable();
    UE_LOG(LogTemp, Log, TEXT("[SkillComp] InitializeSkills: SkillTableSize = %d"), AllRows.Num());

    if (AllRows.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("SkillComponent: SkillTable is empty"));
        return;
    }

    SkillMap.Empty();

    for (const FURPSkillRow& Row : AllRows)
    {
        // 직업 필터
        if (Row.RequiredClass != ClassType)
            continue;

        UURPSkillBase* Skill = nullptr;

        switch (Row.SkillType)
        {
        case EURPSkillType::Projectile:
        {
            Skill = NewObject<UURPProjectileBase>(this);
            auto* Proj = Cast<UURPProjectileBase>(Skill);
            Proj->ProjectileClass = Row.ProjectileClass;
            Proj->DamageMultiplier = Row.DamageMultiplier;
            Proj->SpawnOffset = Row.SpawnOffset;
            break;
        }
        case EURPSkillType::SingleHit:
        {
            Skill = NewObject<UURPSingleHitBase>(this);
            auto* Single = Cast<UURPSingleHitBase>(Skill);
            Single->Range = Row.Range;
            Single->Radius = Row.Radius;
            Single->DamageMultiplier = Row.DamageMultiplier;
            break;
        }
        case EURPSkillType::AOE:
        {
            Skill = NewObject<UURPAOEBase>(this);
            auto* AOE = Cast<UURPAOEBase>(Skill);
            AOE->Radius = Row.Radius;
            AOE->DamageMultiplier = Row.DamageMultiplier;
            break;
        }
        case EURPSkillType::Dash:
        {
            Skill = NewObject<UURPDashBase>(this);
            auto* Dash = Cast<UURPDashBase>(Skill);
            Dash->DashDistance = Row.DashDistance;
            Dash->HitRadius = Row.HitRadius;
            Dash->DamageMultiplier = Row.DamageMultiplier;
            break;
        }
        default:
            break;
        }

        if (!Skill)
            continue;

        Skill->SkillId = Row.SkillId;
        Skill->Cooldown = Row.Cooldown;
        Skill->SkillAnim = Row.SkillAnim;

        RegisterSkill(Skill->SkillId, Skill);
        UE_LOG(LogTemp, Log, TEXT("[SkillComp] Registered skill %s (Id=%d)"),
            *Row.SkillName, Row.SkillId);
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

void UURPSkillComponent::InitializeDefaultSlots(EURPClassType ClassType)
{
    UURPGameDataSubsystem* GameData = GetWorld()->GetGameInstance()->GetSubsystem<UURPGameDataSubsystem>();
    if (!GameData) return;

    const auto& AllSkills = GameData->GetSkillTable();

    TArray<int32> DefaultSlots;

    for (const auto& Row : AllSkills)
    {
        if (Row.RequiredClass == ClassType)
        {
            DefaultSlots.Add(Row.SkillId);
            if (DefaultSlots.Num() >= 4) break;
        }
    }

    // 정렬 (스킬 ID 기반)
    DefaultSlots.Sort();

    SetSkillSlots(DefaultSlots);
}

void UURPSkillComponent::ApplySlotsFromPlayerData(const TArray<FSkillEntry>& SkillLevels)
{
    TArray<int32> Slots;
    for (int i = 0; i < SkillLevels.Num() && i < 4; i++)
    {
        Slots.Add(SkillLevels[i].SkillId);
    }
    SetSkillSlots(Slots);
}

void UURPSkillComponent::EnsureValidSkillSlots(
    EURPClassType ClassType,
    TArray<FSkillEntry>& SkillLevels)
{
    if (SkillLevels.Num() > 0)
    {
        // PlayerData 있음 → 유효 Slot로 판단
        ApplySlotsFromPlayerData(SkillLevels);
        return;
    }

    // PlayerData 없음 → 기본 슬롯 생성
    UURPGameDataSubsystem* GameData = GetWorld()->GetGameInstance()->GetSubsystem<UURPGameDataSubsystem>();
    if (!GameData) return;

    const auto& AllSkills = GameData->GetSkillTable();

    for (const auto& Row : AllSkills)
    {
        if (Row.RequiredClass == ClassType)
        {
            SkillLevels.Add({ Row.SkillId, 1 });
        }
    }

    SkillLevels.Sort([](const FSkillEntry& A, const FSkillEntry& B) {
        return A.SkillId < B.SkillId;
        });

    // 슬롯 적용
    ApplySlotsFromPlayerData(SkillLevels);
}

void UURPSkillComponent::SetSkillSlots(const TArray<int32>& InSlots)
{
    SkillSlots = InSlots;

    // 슬롯 크기는 고정(필요시 자동 확장)
    if (SkillSlots.Num() < 4)
        SkillSlots.SetNum(4);

    UE_LOG(LogTemp, Log, TEXT("[SkillComponent] SkillSlots Set: %d %d %d %d"),
        SkillSlots[0], SkillSlots[1], SkillSlots[2], SkillSlots[3]);
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