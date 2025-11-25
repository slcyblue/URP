#include "URPBuffBase.h"
#include "Core/GamePlay/Mechanics/Status/URPBuffDebuffComponent.h"

void UURPBuffBase::Execute(AURPCharacterBase* Owner)
{
    if (!Owner) return;

    UURPBuffDebuffComponent* BuffComp =
        Owner->FindComponentByClass<UURPBuffDebuffComponent>();

    if (!BuffComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("[BuffSkill] No BuffDebuffComponent on actor."));
        return;
    }

    BuffComp->ApplyBuff(BuffType, BuffValue, Duration);
}
