#include "URPDashBase.h"
#include "Characters/URPMonsterCharacter.h"

void UURPDashBase::Execute(AURPPlayerCharacter* Owner)
{
    if (!Owner || !Owner->HasAuthority()) return;

    // 1) 대시 이동
    FVector NewLoc = Owner->GetActorLocation() +
        Owner->GetActorForwardVector() * DashDistance;

    Owner->SetActorLocation(NewLoc, true);

    // 2) 전진 경로에서 히트 체크
    TArray<FHitResult> Hits;
    FVector Start = Owner->GetActorLocation();
    FVector End = Start + Owner->GetActorForwardVector() * 200.f;

    Owner->GetWorld()->SweepMultiByChannel(
        Hits, Start, End, FQuat::Identity, ECC_Pawn,
        FCollisionShape::MakeSphere(HitRadius));

    for (auto& Hit : Hits)
    {
        if (auto* Monster = Cast<AURPMonsterCharacter>(Hit.GetActor()))
        {
            float Damage = Owner->AttackPower * DamageMultiplier;
            Monster->ApplyDamage(Damage);
        }
    }
}