#include "URPSingleHitBase.h"
#include "Characters/URPMonsterCharacter.h"
#include <Kismet/KismetSystemLibrary.h>

void UURPSingleHitBase::Execute(AURPPlayerCharacter* Owner)
{
    {
        OwnerPC = Owner;

        FVector Start = Owner->GetActorLocation() + FVector(0, 0, 50);
        FVector End = Start + Owner->GetActorForwardVector() * Range;

        TArray<FHitResult> Hits;
        Owner->GetWorld()->SweepMultiByChannel(
            Hits, Start, End, FQuat::Identity, ECC_Pawn,
            FCollisionShape::MakeSphere(Radius)
        );

        for (auto& Hit : Hits)
        {
            if (AURPCharacterBase* Target = Cast<AURPCharacterBase>(Hit.GetActor()))
            {
                float Damage = GetDamage(Owner) * DamageMultiplier;
                Target->ApplyDamage(Damage);
                break;
            }
        }
    }
}
