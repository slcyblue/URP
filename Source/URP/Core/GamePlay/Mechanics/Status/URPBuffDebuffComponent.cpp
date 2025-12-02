#include "URPBuffDebuffComponent.h"
#include "Characters/Common/URPCharacterBase.h"

UURPBuffDebuffComponent::UURPBuffDebuffComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UURPBuffDebuffComponent::BeginPlay()
{
    Super::BeginPlay();
    OwnerCharacter = Cast<AURPCharacterBase>(GetOwner());
}

void UURPBuffDebuffComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* Func)
{
    Super::TickComponent(DeltaTime, TickType, Func);

    // ---- BUFF ----
    for (int32 i = ActiveBuffs.Num() - 1; i >= 0; --i)
    {
        ActiveBuffs[i].RemainingTime -= DeltaTime;
        if (ActiveBuffs[i].RemainingTime <= 0.f)
            ActiveBuffs.RemoveAt(i);
    }

    // ---- DEBUFF ----
    for (int32 i = ActiveDebuffs.Num() - 1; i >= 0; --i)
    {
        ActiveDebuffs[i].RemainingTime -= DeltaTime;
        if (ActiveDebuffs[i].RemainingTime <= 0.f)
            ActiveDebuffs.RemoveAt(i);
    }

    // ---- DOT ----
    for (int32 i = ActiveDOTs.Num() - 1; i >= 0; --i)
    {
        FURPActiveDOT& Dot = ActiveDOTs[i];

        Dot.RemainingTime -= DeltaTime;
        Dot.ElapsedForTick += DeltaTime;

        while (Dot.ElapsedForTick >= Dot.TickInterval && Dot.RemainingTime > 0.f)
        {
            Dot.ElapsedForTick -= Dot.TickInterval;

            if (OwnerCharacter)
                OwnerCharacter->ApplyDamage(Dot.TickDamage);
        }

        if (Dot.RemainingTime <= 0.f)
            ActiveDOTs.RemoveAt(i);
    }
}

// =============================================================
// Apply
// =============================================================

void UURPBuffDebuffComponent::ApplyBuff(EURPBuffType Type, float Value, float Duration)
{
    ActiveBuffs.Add(FURPActiveBuff(Type, Value, Duration));
}

void UURPBuffDebuffComponent::ApplyDebuff(EURPDebuffType Type, float Value, float Duration)
{
    ActiveDebuffs.Add(FURPActiveDebuff(Type, Value, Duration));
}

void UURPBuffDebuffComponent::ApplyDOT(float TickDamage, float TickInterval, float Duration)
{
    ActiveDOTs.Add(FURPActiveDOT(TickDamage, TickInterval, Duration));
}

// =============================================================
// Raw Buff Values (StatComponent에서 최종 계산에 사용)
// =============================================================

float UURPBuffDebuffComponent::GetTotalBuffValue(EURPBuffType Type) const
{
    float Sum = 0.f;
    for (auto& B : ActiveBuffs)
        if (B.Type == Type)
            Sum += B.Value;
    return Sum;
}

float UURPBuffDebuffComponent::GetTotalDebuffValue(EURPDebuffType Type) const
{
    float Sum = 0.f;
    for (auto& D : ActiveDebuffs)
        if (D.Type == Type)
            Sum += D.Value;
    return Sum;
}

float UURPBuffDebuffComponent::GetFlat(EURPBuffType Type) const
{
    return GetTotalBuffValue(Type);
}

float UURPBuffDebuffComponent::GetPercent(EURPBuffType Type) const
{
    return PercentToRate(GetTotalBuffValue(Type));
}

// =============================================================
// States
// =============================================================

bool UURPBuffDebuffComponent::IsStunned() const
{
    for (auto& D : ActiveDebuffs)
        if (D.Type == EURPDebuffType::Stun)
            return true;
    return false;
}

bool UURPBuffDebuffComponent::IsSilenced() const
{
    for (auto& D : ActiveDebuffs)
        if (D.Type == EURPDebuffType::Silence)
            return true;
    return false;
}
