#include "URPSingleHitBase.h"
#include "Characters/URPMonsterCharacter.h"

void UURPSingleHitBase::Execute(AURPPlayerCharacter* Owner)
{
    if (!Owner || !Owner->HasAuthority()) return;

    FVector Start = Owner->GetActorLocation();
    FVector End = Start + Owner->GetActorForwardVector() * Range;

    FHitResult Hit;
    FCollisionShape Shape = FCollisionShape::MakeSphere(Radius);
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);

    if (Owner->GetWorld()->SweepSingleByChannel(Hit, Start, End, Owner->GetActorQuat(),
        ECC_Pawn, Shape, Params))
    {
        if (auto* Monster = Cast<AURPMonsterCharacter>(Hit.GetActor()))
        {
            float Damage = Owner->AttackPower * DamageMultiplier;
            Monster->ApplyDamage(Damage);
        }
    }
}
