#include "URPBuffDebuffComponent.h"
#include "GameFramework/Actor.h"

// PercentValue: 20.0 => 0.2 (+20%)
float UURPBuffDebuffComponent::PercentToRate(float PercentValue)
{
    return PercentValue * 0.01f;
}

// ================= Apply =================

void UURPBuffDebuffComponent::ApplyBuff(EURPBuffType Type, float Value, float Duration)
{
    if (Type == EURPBuffType::None || Duration <= 0.f)
        return;

    // 같은 타입이면 갱신(가장 센 값 유지 + 시간 갱신)
    for (FURPActiveBuff& B : ActiveBuffs)
    {
        if (B.Type == Type)
        {
            B.Value = FMath::Max(B.Value, Value);
            B.RemainingTime = FMath::Max(B.RemainingTime, Duration);
            return;
        }
    }

    FURPActiveBuff NewBuff;
    NewBuff.Type = Type;
    NewBuff.Value = Value;
    NewBuff.RemainingTime = Duration;
    ActiveBuffs.Add(NewBuff);
}

void UURPBuffDebuffComponent::ApplyDebuff(EURPDebuffType Type, float Value, float Duration)
{
    if (Type == EURPDebuffType::None || Duration <= 0.f)
        return;

    for (FURPActiveDebuff& D : ActiveDebuffs)
    {
        if (D.Type == Type)
        {
            D.Value = FMath::Max(D.Value, Value);
            D.RemainingTime = FMath::Max(D.RemainingTime, Duration);
            return;
        }
    }

    FURPActiveDebuff NewDebuff;
    NewDebuff.Type = Type;
    NewDebuff.Value = Value;
    NewDebuff.RemainingTime = Duration;
    ActiveDebuffs.Add(NewDebuff);
}

void UURPBuffDebuffComponent::ApplyDOT(float TickDamage, float TickInterval, float Duration)
{
    if (TickDamage <= 0.f || TickInterval <= 0.f || Duration <= 0.f)
        return;

    FURPActiveDOT NewDot;
    NewDot.TickDamage = TickDamage;
    NewDot.TickInterval = TickInterval;
    NewDot.RemainingTime = Duration;
    NewDot.ElapsedForTick = 0.f;

    ActiveDOTs.Add(NewDot);
}

// ================= Multiplier =================

float UURPBuffDebuffComponent::GetAttackMultiplier() const
{
    float AddRate = 0.f;

    for (const FURPActiveBuff& B : ActiveBuffs)
    {
        if (B.Type == EURPBuffType::AttackUp)
        {
            AddRate += PercentToRate(B.Value);
        }
    }
    for (const FURPActiveDebuff& D : ActiveDebuffs)
    {
        if (D.Type == EURPDebuffType::AttackDown)
        {
            AddRate -= PercentToRate(D.Value);
        }
    }

    return FMath::Max(0.1f, 1.f + AddRate);
}

float UURPBuffDebuffComponent::GetDefenseMultiplier() const
{
    float AddRate = 0.f;

    for (const FURPActiveBuff& B : ActiveBuffs)
    {
        if (B.Type == EURPBuffType::DefenseUp)
        {
            AddRate += PercentToRate(B.Value);
        }
    }
    for (const FURPActiveDebuff& D : ActiveDebuffs)
    {
        if (D.Type == EURPDebuffType::DefenseDown)
        {
            AddRate -= PercentToRate(D.Value);
        }
    }

    return FMath::Max(0.1f, 1.f + AddRate);
}

float UURPBuffDebuffComponent::GetMoveSpeedMultiplier() const
{
    float AddRate = 0.f;

    for (const FURPActiveBuff& B : ActiveBuffs)
    {
        if (B.Type == EURPBuffType::MoveSpeedUp)
        {
            AddRate += PercentToRate(B.Value);
        }
    }
    for (const FURPActiveDebuff& D : ActiveDebuffs)
    {
        if (D.Type == EURPDebuffType::MoveSpeedDown)
        {
            AddRate -= PercentToRate(D.Value);
        }
    }

    return FMath::Max(0.1f, 1.f + AddRate);
}

float UURPBuffDebuffComponent::GetSkillHasteMultiplier() const
{
    float AddRate = 0.f;

    for (const FURPActiveBuff& B : ActiveBuffs)
    {
        if (B.Type == EURPBuffType::SkillHaste)
        {
            AddRate += PercentToRate(B.Value);
        }
    }

    // SkillHaste만 Buff로 가정 (쿨타임 감소)
    return FMath::Max(0.f, 1.f - AddRate);
}

// ================= 상태 쿼리 =================

bool UURPBuffDebuffComponent::IsStunned() const
{
    for (const FURPActiveDebuff& D : ActiveDebuffs)
    {
        if (D.Type == EURPDebuffType::Stun)
            return true;
    }
    return false;
}

bool UURPBuffDebuffComponent::IsSilenced() const
{
    for (const FURPActiveDebuff& D : ActiveDebuffs)
    {
        if (D.Type == EURPDebuffType::Silence)
            return true;
    }
    return false;
}
