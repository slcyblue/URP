#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <URPGameTypes.h>
#include "URPStatComponent.generated.h"

class AURPCharacterBase;
class UURPBuffDebuffComponent;

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
    void SetBaseStats(float InMaxHp = 100, float InAttack = 10, float InDefense = 5, float InAttackSpeed = 1, float InMoveSpeed =1);

    /** 장비 변경 시 장비 스탯 설정 (EquipmentComponent에서 호출) */
    void SetEquipStats(int64 InEquipMaxHp, float InEquipAttack, float InEquipDefense);

    /** 최종 스탯 재계산 (Class / Equip / Buff 변경시 호출) */
    void Recalculate();

    /** 최종 스탯 Getter (필요시 외부에서 직접 사용 가능) */
    float GetFinalMaxHp() const { return FinalMaxHp; }
    float GetFinalAttack() const { return FinalAttack; }
    float GetFinalDefense() const { return FinalDefense; }
    float GetFinalAttackSpeed() const { return FinalAttackSpeed; }

protected:
    virtual void BeginPlay() override;

private:
    /** 소유 캐릭터 (플레이어/몬스터 공용) */
    UPROPERTY()
    AURPCharacterBase* OwnerCharacter = nullptr;

    UPROPERTY()
    UURPBuffDebuffComponent* BuffComp = nullptr;

    // 최종 스탯
    float FinalMaxHp = 0.f;
    float FinalAttack = 0.f;
    float FinalDefense = 0.f;
    float FinalMoveSpeed = 600.f;
    float FinalAttackSpeed = 1.f;

    // 기본 스탯
    float BaseMaxHp = 10.f;
    float BaseAttack = 10.f;
    float BaseDefense = 5.f;
    float BaseMoveSpeed = 600.f;
    float BaseAttackSpeed = 1.f;

    // 장비 스탯
    float EquipMaxHp = 0.f;
    float EquipAttack = 0.f;
    float EquipDefense = 0.f;
    float EquipMoveSpeed = 0.f;
    float EquipAttackSpeed = 0.f;
};
