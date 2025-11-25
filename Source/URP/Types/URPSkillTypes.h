#pragma once
#include "CoreMinimal.h"
#include "URPCommonEnums.h"
#include "URPSkillTypes.generated.h"

USTRUCT(BlueprintType)
struct URP_API FURPSkillRow
{
    GENERATED_BODY()

    UPROPERTY() int32 SkillId;
    UPROPERTY() FString SkillName;
    UPROPERTY() EURPSkillType SkillType;
    UPROPERTY() EURPClassType RequiredClass;
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

    UPROPERTY() EURPBuffType Type;
    UPROPERTY() float Value;
    UPROPERTY() float RemainingTime;
};

USTRUCT()
struct URP_API FURPActiveDebuff
{
    GENERATED_BODY()

    UPROPERTY() EURPDebuffType Type;
    UPROPERTY() float Value;
    UPROPERTY() float RemainingTime;
};


USTRUCT()
struct URP_API FURPActiveDOT
{
    GENERATED_BODY()

    UPROPERTY() float TickDamage = 0.f;
    UPROPERTY() float TickInterval = 1.f;
    UPROPERTY() float RemainingTime = 0.f;

    // 내부용
    float ElapsedForTick = 0.f;
};

USTRUCT()
struct URP_API FDOTData
{
    GENERATED_BODY()

    UPROPERTY() int32 SkillId;
    UPROPERTY() FString DotType;
    UPROPERTY() float Duration;
    UPROPERTY() float TickInterval;
    UPROPERTY() float DamagePerTick;
};
