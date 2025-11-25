#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "URPStatComponent.generated.h"

class AURPCharacterBase;

/**
 * 스탯 전담 컴포넌트
 * - Base / Equip / Buff 등의 소스를 합산해서 최종 스탯 계산
 * - 결과는 Owner(AURPCharacterBase)의 MaxHp / AttackPower / DefensePower에 반영
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class URP_API UURPStatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UURPStatComponent();

    /** 클래스 변경 시 기본 스탯 설정 */
    void SetBaseStats(int64 InBaseMaxHp, float InBaseAttack, float InBaseDefense);

    /** 장비 변경 시 장비 스탯 설정 (EquipmentComponent에서 호출) */
    void SetEquipStats(int64 InEquipMaxHp, float InEquipAttack, float InEquipDefense);

    /** 추후 Buff/Debuff용으로 확장 가능(지금은 0으로 두고 시작해도 됨) */
    void SetBuffStats(int64 InBuffMaxHp, float InBuffAttack, float InBuffDefense);

    /** 최종 스탯 재계산 (Class / Equip / Buff 변경시 호출) */
    void Recalculate();

    /** 최종 스탯 Getter (필요시 외부에서 직접 사용 가능) */
    int64  GetFinalMaxHp() const { return FinalMaxHp; }
    float  GetFinalAttack() const { return FinalAttack; }
    float  GetFinalDefense() const { return FinalDefense; }

protected:
    virtual void BeginPlay() override;

private:
    /** 소유 캐릭터 (플레이어/몬스터 공용) */
    UPROPERTY()
    AURPCharacterBase* OwnerCharacter = nullptr;

    // ===== Base (클래스 등에서 제공) =====
    int64 BaseMaxHp = 100;
    float BaseAttack = 10.f;
    float BaseDefense = 5.f;

    // ===== Equip (장비에서 제공) =====
    int64 EquipMaxHp = 0;
    float EquipAttack = 0.f;
    float EquipDefense = 0.f;

    // ===== Buff / Debuff (추가 스탯, 옵션용) =====
    int64 BuffMaxHp = 0;
    float BuffAttack = 0.f;
    float BuffDefense = 0.f;

    // ===== 최종 계산된 값 =====
    int64 FinalMaxHp = 100;
    float FinalAttack = 10.f;
    float FinalDefense = 5.f;

    void SyncFromOwnerIfNeeded();
};
