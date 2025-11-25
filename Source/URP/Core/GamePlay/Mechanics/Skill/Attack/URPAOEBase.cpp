#include "URPAOEBase.h"
#include "Characters/Monster/URPMonsterCharacter.h"
#include <Kismet/KismetSystemLibrary.h>

void UURPAOEBase::Execute(AURPCharacterBase* Owner)
{
    OwnerPC = Owner;
    CurrentTick = 0;

    if (Delay > 0.f)
    {
        Owner->GetWorldTimerManager().SetTimer(
            TickHandle, this, &UURPAOEBase::StartTick, Delay, false);
    }
    else
    {
        StartTick();
    }
}

void UURPAOEBase::StartTick()
{
    OwnerPC->GetWorldTimerManager().SetTimer(
        TickHandle, this, &UURPAOEBase::DoAOETick, TickInterval, true);
}

void UURPAOEBase::DoAOETick()
{
    CurrentTick++;
    if (CurrentTick > MaxTicks)
    {
        OwnerPC->GetWorldTimerManager().ClearTimer(TickHandle);
        return;
    }

    FVector Center = OwnerPC->GetActorLocation();

    TArray<AActor*> Hits;
    UKismetSystemLibrary::SphereOverlapActors(
        OwnerPC->GetWorld(),
        Center,
        Radius,
        { UEngineTypes::ConvertToObjectType(ECC_Pawn) },
        AURPCharacterBase::StaticClass(),
        { OwnerPC },
        Hits
    );

    for (AActor* Hit : Hits)
    {
        AURPCharacterBase* Target = Cast<AURPCharacterBase>(Hit);
        if (Target)
        {
            float Dmg = GetDamage(OwnerPC) * DamageMultiplier;
            Target->ApplyDamage(Dmg);
        }
    }
}