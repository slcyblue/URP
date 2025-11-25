#include "URPStatComponent.h"
#include "Characters/Common/URPCharacterBase.h"

UURPStatComponent::UURPStatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UURPStatComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerCharacter = Cast<AURPCharacterBase>(GetOwner());
    SyncFromOwnerIfNeeded();
    Recalculate();
}

void UURPStatComponent::SyncFromOwnerIfNeeded()
{
    if (!OwnerCharacter) return;

    // 만약 기존 CharacterBase에 초깃값이 세팅되어 있다면 그것을 Base로 가져올 수 있음
    BaseMaxHp = OwnerCharacter->BaseMaxHp;
    BaseAttack = OwnerCharacter->BaseAttack;
    BaseDefense = OwnerCharacter->BaseDefense;

    EquipMaxHp = OwnerCharacter->EquipMaxHp;
    EquipAttack = OwnerCharacter->EquipAttack;
    EquipDefense = OwnerCharacter->EquipDefense;
}

void UURPStatComponent::SetBaseStats(int64 InBaseMaxHp, float InBaseAttack, float InBaseDefense)
{
    BaseMaxHp = InBaseMaxHp;
    BaseAttack = InBaseAttack;
    BaseDefense = InBaseDefense;

    Recalculate();
}

void UURPStatComponent::SetEquipStats(int64 InEquipMaxHp, float InEquipAttack, float InEquipDefense)
{
    EquipMaxHp = InEquipMaxHp;
    EquipAttack = InEquipAttack;
    EquipDefense = InEquipDefense;

    Recalculate();
}

void UURPStatComponent::SetBuffStats(int64 InBuffMaxHp, float InBuffAttack, float InBuffDefense)
{
    BuffMaxHp = InBuffMaxHp;
    BuffAttack = InBuffAttack;
    BuffDefense = InBuffDefense;

    Recalculate();
}

void UURPStatComponent::Recalculate()
{
    if (!OwnerCharacter) return;

    // 일단은 단순 합산: 나중에 퍼센트 버프가 필요하면 여기서 곱연산 추가
    FinalMaxHp = BaseMaxHp + EquipMaxHp + BuffMaxHp;
    FinalAttack = BaseAttack + EquipAttack + BuffAttack;
    FinalDefense = BaseDefense + EquipDefense + BuffDefense;

    // CharacterBase에 반영
    OwnerCharacter->MaxHp = FinalMaxHp;
    OwnerCharacter->AttackPower = FinalAttack;
    OwnerCharacter->DefensePower = FinalDefense;

    // 현재 HP 보정
    if (OwnerCharacter->CurrentHp > FinalMaxHp)
        OwnerCharacter->CurrentHp = FinalMaxHp;

    UE_LOG(LogTemp, Log, TEXT("[StatComponent] Recalc: MaxHp=%lld, Atk=%.1f, Def=%.1f"),
        FinalMaxHp, FinalAttack, FinalDefense);
}
