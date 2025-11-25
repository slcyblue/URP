#include "URPSingleHitBase.h"
#include "Characters/Monster/URPMonsterCharacter.h"
#include <Kismet/KismetSystemLibrary.h>

void UURPSingleHitBase::Execute(AURPCharacterBase* Owner)
{
    OwnerPC = Owner;

    FVector Start = Owner->GetActorLocation() + FVector(0, 0, 50);
    FVector End = Start + Owner->GetActorForwardVector() * Range;

    TArray<FHitResult> Hits;
    Owner->GetWorld()->SweepMultiByChannel(
        Hits, Start, End, FQuat::Identity, ECC_Pawn,
        FCollisionShape::MakeSphere(Radius)
    );

    float Damage = GetDamage(Owner) * DamageMultiplier;

    for (auto& Hit : Hits)
    {
        if (AURPCharacterBase* Target = Cast<AURPCharacterBase>(Hit.GetActor()))
        {
            Target->ApplyDamage(Damage);
            break;
        }
    }
}
