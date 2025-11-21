#include "URPAOEBase.h"
#include "Characters/URPMonsterCharacter.h"

void UURPAOEBase::Execute(AURPPlayerCharacter* Owner)
{
    if (!Owner || !Owner->HasAuthority()) return;

    TArray<FHitResult> Hits;
    FVector Center = Owner->GetActorLocation();

    bool bHit = Owner->GetWorld()->SweepMultiByChannel(
        Hits, Center, Center,
        FQuat::Identity, ECC_Pawn,
        FCollisionShape::MakeSphere(Radius));

    if (!bHit) return;

    for (auto& Hit : Hits)
    {
        if (auto* Monster = Cast<AURPMonsterCharacter>(Hit.GetActor()))
        {
            float Damage = Owner->AttackPower * DamageMultiplier;
            Monster->ApplyDamage(Damage);
        }
    }
}