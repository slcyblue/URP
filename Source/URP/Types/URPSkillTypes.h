#pragma once
#include "CoreMinimal.h"
#include "URPCommonEnums.h"
#include "URPSkillTypes.generated.h"


USTRUCT(BlueprintType)
struct URP_API FSkillEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SkillId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Level;
};



USTRUCT(BlueprintType)
struct URP_API FURPSkillRow
{
    GENERATED_BODY()

    UPROPERTY() int32 SkillId;
    UPROPERTY() FString SkillName;
    UPROPERTY() EURPSkillType SkillType;
    UPROPERTY() EURPClassType RequiredClass;
    UPROPERTY() FString SkillAnimPath;
    UPROPERTY() UAnimMontage* SkillAnim = nullptr;
    UPROPERTY() float Cooldown;
    UPROPERTY() int32 ApplyDotSkillId;

    // Projectile
    UPROPERTY() FString ProjectileClassPath;
    UPROPERTY() TSubclassOf<AActor> ProjectileClass;
    UPROPERTY() FVector SpawnOffset;

    // SingleHit/AOE
    UPROPERTY() float Range;
    UPROPERTY() float Radius;
    UPROPERTY() float DamageMultiplier;

    // Dash
    UPROPERTY() float DashDistance;
    UPROPERTY() float HitRadius;

    // DOT
    UPROPERTY() float TickDamage;
    UPROPERTY() float TickInterval;
    UPROPERTY() int32 TickCount;

    // Buff
    UPROPERTY() EURPBuffType BuffType;
    UPROPERTY() float BuffValue;
    UPROPERTY() float Duration;

    // Debuff
    UPROPERTY() EURPDebuffType DebuffType;
    UPROPERTY() float DebuffValue;
};


USTRUCT()
struct URP_API FURPActiveBuff
{
    GENERATED_BODY()

    UPROPERTY()
    EURPBuffType Type = EURPBuffType::None;

    UPROPERTY()
    float Value = 0.f;        // Flat or Percent (버프 종류에 따라 용도 다름)

    UPROPERTY()
    float Duration = 0.f;

    UPROPERTY()
    float RemainingTime = 0.f;

    FURPActiveBuff() {}

    FURPActiveBuff(EURPBuffType InType, float InValue, float InDuration)
    {
        Type = InType;
        Value = InValue;
        Duration = InDuration;
        RemainingTime = InDuration;
    }
};

USTRUCT()
struct URP_API FURPActiveDebuff
{
    GENERATED_BODY()

    UPROPERTY()
    EURPDebuffType Type = EURPDebuffType::None;

    UPROPERTY()
    float Value = 0.f;

    UPROPERTY()
    float Duration = 0.f;

    UPROPERTY()
    float RemainingTime = 0.f;

    FURPActiveDebuff() {}

    FURPActiveDebuff(EURPDebuffType InType, float InValue, float InDuration)
    {
        Type = InType;
        Value = InValue;
        Duration = InDuration;
        RemainingTime = InDuration;
    }
};

USTRUCT()
struct URP_API FURPActiveDOT
{
    GENERATED_BODY()

    UPROPERTY()
    float TickDamage = 0.f;

    UPROPERTY()
    float TickInterval = 1.f;

    UPROPERTY()
    float RemainingTime = 0.f;

    float ElapsedForTick = 0.f;

    FURPActiveDOT() {}

    FURPActiveDOT(float Damage, float Interval, float Duration)
    {
        TickDamage = Damage;
        TickInterval = Interval;
        RemainingTime = Duration;
        ElapsedForTick = 0.f;
    }
};