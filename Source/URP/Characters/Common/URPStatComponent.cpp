#include "URPStatComponent.h"
#include "Core/GamePlay/Mechanics/Status/URPBuffDebuffComponent.h"
#include "Characters/Common/URPCharacterBase.h"

UURPStatComponent::UURPStatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UURPStatComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerCharacter = Cast<AURPCharacterBase>(GetOwner());
    BuffComp = OwnerCharacter ? OwnerCharacter->BuffDebuffComp : nullptr;
    Recalculate();
}

void UURPStatComponent::SetBaseStats(float InMaxHp, float InAttack, float InDefense, float InAttackSpeed, float InMoveSpeed)
{
    BaseMaxHp = InMaxHp;
    BaseAttack = InAttack;
    BaseDefense = InDefense;
    BaseAttackSpeed = InAttackSpeed;
    BaseMoveSpeed = InMoveSpeed;

    Recalculate();
}

void UURPStatComponent::SetEquipStats(int64 InEquipMaxHp, float InEquipAttack, float InEquipDefense)
{
    EquipMaxHp = InEquipMaxHp;
    EquipAttack = InEquipAttack;
    EquipDefense = InEquipDefense;

    Recalculate();
}


void UURPStatComponent::Recalculate()
{
    if (!OwnerCharacter || !BuffComp)
        return;

    // ============================================================
    // 1) Attack
    // ============================================================
    float BuffAtkFlat = BuffComp->GetFlat(EURPBuffType::AttackUp);
    float BuffAtkPercent = BuffComp->GetPercent(EURPBuffType::AttackUp);

    float DebuffAtkFlat = BuffComp->GetTotalDebuffValue(EURPDebuffType::AttackDown);
    float DebuffAtkPercent = BuffComp->GetTotalDebuffValue(EURPDebuffType::AttackDown) * 0.01f;

    FinalAttack =
        (BaseAttack + EquipAttack + BuffAtkFlat - DebuffAtkFlat)
        * (1.f + BuffAtkPercent - DebuffAtkPercent);

    FinalAttack = FMath::Max(FinalAttack, 1.f); // 최소 1 공격력 보정


    // ============================================================
    // 2) Defense
    // ============================================================
    float BuffDefFlat = BuffComp->GetFlat(EURPBuffType::DefenseUp);
    float BuffDefPercent = BuffComp->GetPercent(EURPBuffType::DefenseUp);

    float DebuffDefFlat = BuffComp->GetTotalDebuffValue(EURPDebuffType::DefenseDown);
    float DebuffDefPercent = BuffComp->GetTotalDebuffValue(EURPDebuffType::DefenseDown) * 0.01f;

    FinalDefense =
        (BaseDefense + EquipDefense + BuffDefFlat - DebuffDefFlat)
        * (1.f + BuffDefPercent - DebuffDefPercent);

    FinalDefense = FMath::Max(FinalDefense, 0.f);


    // ============================================================
    // 3) MoveSpeed (슬로우 포함)
    // ============================================================
    float BuffMoveFlat = BuffComp->GetFlat(EURPBuffType::MoveSpeedUp);
    float BuffMovePercent = BuffComp->GetPercent(EURPBuffType::MoveSpeedUp);

    float DebuffMoveFlat = BuffComp->GetTotalDebuffValue(EURPDebuffType::MoveSpeedDown);
    float DebuffMovePercent = BuffComp->GetTotalDebuffValue(EURPDebuffType::MoveSpeedDown) * 0.01f;

    FinalMoveSpeed =
        (BaseMoveSpeed + EquipMoveSpeed + BuffMoveFlat - DebuffMoveFlat)
        * (1.f + BuffMovePercent - DebuffMovePercent);

    FinalMoveSpeed = FMath::Clamp(FinalMoveSpeed, 150.f, 2000.f);


    // ============================================================
    // 4) AttackSpeed (공속 버프/공속 디버프 모두 포함)
    // ============================================================
    float BuffASFlat = BuffComp->GetFlat(EURPBuffType::AttackSpeedUp);
    float BuffASPercent = BuffComp->GetPercent(EURPBuffType::AttackSpeedUp);

    float DebuffASFlat = BuffComp->GetTotalDebuffValue(EURPDebuffType::AttackSpeedDown);
    float DebuffASPercent = BuffComp->GetTotalDebuffValue(EURPDebuffType::AttackSpeedDown) * 0.01f;

    FinalAttackSpeed =
        (BaseAttackSpeed + EquipAttackSpeed + BuffASFlat - DebuffASFlat)
        * (1.f + BuffASPercent - DebuffASPercent);

    FinalAttackSpeed = FMath::Clamp(FinalAttackSpeed, 0.1f, 5.f);

    // 5) MaxHP

    float BuffHPFlat = BuffComp->GetFlat(EURPBuffType::MaxHPUp);
    float BuffHPPercent = BuffComp->GetPercent(EURPBuffType::MaxHPUp);

    float DebuffHPFlat = BuffComp->GetTotalDebuffValue(EURPDebuffType::MaxHPDown);
    float DebuffHPPercent = BuffComp->GetTotalDebuffValue(EURPDebuffType::MaxHPDown) * 0.01f;

    FinalMaxHp =
        (BaseMaxHp + EquipMaxHp + BuffHPFlat - DebuffHPFlat)
        * (1.f + BuffHPPercent - DebuffHPPercent);

    FinalMaxHp = FMath::Max(FinalAttackSpeed, 100.f);
}
