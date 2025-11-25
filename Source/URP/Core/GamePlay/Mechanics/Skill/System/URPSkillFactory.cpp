#include "URPSkillFactory.h"
#include "Core/GamePlay/Mechanics/Skill/Base/URPSkillBase.h"
#include "Core/GamePlay/Mechanics/Skill/Base/URPDOTBase.h"
#include "Core/GamePlay/Mechanics/Skill/Base/URPBuffBase.h"
#include "Core/GamePlay/Mechanics/Skill/Base/URPDebuffBase.h"
#include "Core/GamePlay/Mechanics/Skill/Attack/URPProjectileBase.h"
#include "Core/GamePlay/Mechanics/Skill/Attack/URPSingleHitBase.h"
#include "Core/GamePlay/Mechanics/Skill/Attack/URPAOEBase.h"
#include "Core/GamePlay/Mechanics/Skill/Attack/URPDashBase.h"

UURPSkillBase* UURPSkillFactory::CreateSkill(const FURPSkillRow& Row, UObject* Outer)
{
    if (!Outer)
    {
        UE_LOG(LogTemp, Error, TEXT("[SkillFactory] Outer is null. SkillId=%d"), Row.SkillId);
        return nullptr;
    }

    UURPSkillBase* NewSkill = nullptr;

    switch (Row.SkillType)
    {
    case EURPSkillType::Projectile:
        NewSkill = CreateProjectileSkill(Row, Outer);
        break;

    case EURPSkillType::SingleHit:
        NewSkill = CreateSingleHitSkill(Row, Outer);
        break;

    case EURPSkillType::AOE:
        NewSkill = CreateAOESkill(Row, Outer);
        break;

    case EURPSkillType::Dash:
        NewSkill = CreateDashSkill(Row, Outer);
        break;

    case EURPSkillType::DOT:
        NewSkill = CreateDOTSkill(Row, Outer);
        break;

    case EURPSkillType::Buff:
        NewSkill = CreateBuffSkill(Row, Outer);
        break;

    case EURPSkillType::Debuff:
        NewSkill = CreateDebuffSkill(Row, Outer);
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("[SkillFactory] Unsupported SkillType for SkillId=%d"), Row.SkillId);
        break;
    }

    if (!NewSkill)
    {
        UE_LOG(LogTemp, Warning, TEXT("[SkillFactory] Failed to create skill instance. SkillId=%d"), Row.SkillId);
        return nullptr;
    }

    // 공통 데이터 세팅
    ApplyCommonData(NewSkill, Row);

    UE_LOG(LogTemp, Log, TEXT("[SkillFactory] Created Skill: %s (Id=%d, Type=%d)"),
        *Row.SkillName, Row.SkillId, static_cast<int32>(Row.SkillType));

    return NewSkill;
}

void UURPSkillFactory::ApplyCommonData(UURPSkillBase* Skill, const FURPSkillRow& Row)
{
    if (!Skill) return;

    Skill->SkillId = Row.SkillId;
    Skill->Cooldown = Row.Cooldown;
    Skill->ApplyDotSkillId = Row.ApplyDotSkillId;
    // TODO: 필요하다면 여기서 추가 공통 필드도 셋팅
    // 예: Skill->RequiredClass = Row.RequiredClass;
    //      Skill->SomeTag = Row.Tag;
}

/** Projectile */
UURPSkillBase* UURPSkillFactory::CreateProjectileSkill(const FURPSkillRow& Row, UObject* Outer)
{
    if (!Row.ProjectileClass)
    {
        UE_LOG(LogTemp, Error, TEXT("[SkillFactory] Projectile skill has no ProjectileClass. SkillId=%d"), Row.SkillId);
        return nullptr;
    }

    UURPProjectileBase* ProjSkill = NewObject<UURPProjectileBase>(Outer);
    if (!ProjSkill) return nullptr;

    ProjSkill->ProjectileClass = Row.ProjectileClass;
    ProjSkill->DamageMultiplier = Row.DamageMultiplier;
    ProjSkill->SpawnOffset = Row.SpawnOffset;

    return ProjSkill;
}

/** SingleHit */
UURPSkillBase* UURPSkillFactory::CreateSingleHitSkill(const FURPSkillRow& Row, UObject* Outer)
{
    UURPSingleHitBase* SingleSkill = NewObject<UURPSingleHitBase>(Outer);
    if (!SingleSkill) return nullptr;

    SingleSkill->Range = Row.Range;
    SingleSkill->Radius = Row.Radius;
    SingleSkill->DamageMultiplier = Row.DamageMultiplier;

    return SingleSkill;
}

/** AOE */
UURPSkillBase* UURPSkillFactory::CreateAOESkill(const FURPSkillRow& Row, UObject* Outer)
{
    UURPAOEBase* AOESkill = NewObject<UURPAOEBase>(Outer);
    if (!AOESkill) return nullptr;

    AOESkill->Radius = Row.Radius;
    AOESkill->DamageMultiplier = Row.DamageMultiplier;

    return AOESkill;
}

/** Dash */
UURPSkillBase* UURPSkillFactory::CreateDashSkill(const FURPSkillRow& Row, UObject* Outer)
{
    UURPDashBase* DashSkill = NewObject<UURPDashBase>(Outer);
    if (!DashSkill) return nullptr;

    DashSkill->DashDistance = Row.DashDistance;
    DashSkill->HitRadius = Row.HitRadius;
    DashSkill->DamageMultiplier = Row.DamageMultiplier;

    return DashSkill;
}

/* ---------------------- DOT ---------------------- */

UURPSkillBase* UURPSkillFactory::CreateDOTSkill(const FURPSkillRow& Row, UObject* Outer)
{
    UURPDOTBase* Skill = NewObject<UURPDOTBase>(Outer);
    Skill->TickDamage = Row.TickDamage;
    Skill->TickInterval = Row.TickInterval;
    //Skill->TickCount = Row.TickCount;
    return Skill;
}

/* ---------------------- Buff ---------------------- */

UURPSkillBase* UURPSkillFactory::CreateBuffSkill(const FURPSkillRow& Row, UObject* Outer)
{
    UURPBuffBase* Skill = NewObject<UURPBuffBase>(Outer);

    Skill->BuffType = Row.BuffType;
    Skill->BuffValue = Row.BuffValue;
    Skill->Duration = Row.Duration;

    return Skill;
}

/* ---------------------- Debuff ---------------------- */

UURPSkillBase* UURPSkillFactory::CreateDebuffSkill(const FURPSkillRow& Row, UObject* Outer)
{
    UURPDebuffBase* Skill = NewObject<UURPDebuffBase>(Outer);

    Skill->DebuffType = Row.DebuffType;
    Skill->DebuffValue = Row.DebuffValue;
    Skill->Duration = Row.Duration;

    return Skill;
}