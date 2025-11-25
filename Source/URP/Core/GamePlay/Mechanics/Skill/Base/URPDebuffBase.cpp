#include "URPDebuffBase.h"
#include "Core/GamePlay/Mechanics/Status/URPBuffDebuffComponent.h"

void UURPDebuffBase::Execute(AURPCharacterBase* Owner)
{
    if (!Owner) return;

    UURPBuffDebuffComponent* BuffComp =
        Owner->FindComponentByClass<UURPBuffDebuffComponent>();

    if (!BuffComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("[DebuffSkill] No BuffDebuffComponent on actor."));
        return;
    }

    BuffComp->ApplyDebuff(DebuffType, DebuffValue, Duration);
}
