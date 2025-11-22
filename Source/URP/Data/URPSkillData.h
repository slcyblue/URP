#pragma once
#include "CoreMinimal.h"
#include "URPCommonEnums.h"
#include "URPSkillData.generated.h"

USTRUCT(BlueprintType)
struct URP_API FURPSkillRow
{
    GENERATED_BODY()

    /** 스킬 고유 ID */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 SkillId = 0;

    /** 에디터/디버그용 이름 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString SkillName;

    /** 어떤 직업 전용 스킬인지 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EURPClassType RequiredClass = EURPClassType::None;

    /** 스킬 동작 방식 타입 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EURPSkillType SkillType = EURPSkillType::SingleHit;

    /** 쿨다운(초) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Cooldown = 3.f;

    /** 데미지 배수 (AttackPower * DamageMultiplier) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DamageMultiplier = 1.0f;

    /** 사거리 / 대시 거리 / 범위 등에서 사용하는 기본 Range */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Range = 200.f;

    /** AOE/SingleHit 사용 시 반지름 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Radius = 100.f;

    /** Dash 전진 거리 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DashDistance = 600.f;

    /** Dash 중 히트 범위 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float HitRadius = 60.f;

    /** Projectile용 투사체 클래스 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString ProjectileClassPath;

    /** Projectile용 투사체 클래스 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AActor> ProjectileClass;

    /** Projectile 스폰 오프셋 (로컬) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector SpawnOffset = FVector(30, 0, 50);
};