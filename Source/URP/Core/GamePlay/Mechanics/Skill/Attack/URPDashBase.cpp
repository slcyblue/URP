#include "URPDashBase.h"
#include "Characters/Monster/URPMonsterCharacter.h"
#include <Kismet/KismetSystemLibrary.h>

void UURPDashBase::Execute(AURPCharacterBase* Owner)
{
    OwnerPC = Owner;

    FVector Start = Owner->GetActorLocation();
    FVector End = Start + Owner->GetActorForwardVector() * DashDistance;

    Owner->SetActorLocation(End, true);

    TArray<FHitResult> Hits;
    Owner->GetWorld()->SweepMultiByChannel(
        Hits, Start, End, FQuat::Identity, ECC_Pawn,
        FCollisionShape::MakeSphere(HitRadius)
    );

    for (auto& Hit : Hits)
    {
        if (AURPCharacterBase* Target = Cast<AURPCharacterBase>(Hit.GetActor()))
        {
            float Damage = GetDamage(Owner) * DamageMultiplier;
            Target->ApplyDamage(Damage);
        }
    }
}