#include "URPDOTBase.h"
#include "Core/GamePlay/Mechanics/Status/URPBuffDebuffComponent.h"
#include "Characters/Common/URPCharacterBase.h"

void UURPDOTBase::ApplyDOT(AURPCharacterBase* Target)
{
    if (!Target) return;

    if (UURPBuffDebuffComponent* BuffComp = Target->FindComponentByClass<UURPBuffDebuffComponent>())
    {
        BuffComp->ApplyDOT(TickDamage, TickInterval, Duration);
    }
}

void UURPDOTBase::Execute(AURPCharacterBase* Owner)
{
    // DOT은 보통 Owner가 자기 자신/타겟에게 적용하게끔 설계됨
    ApplyDOT(Owner);
}