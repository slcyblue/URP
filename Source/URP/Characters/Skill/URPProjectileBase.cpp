#include "URPProjectileBase.h"
#include "Characters/URPMonsterCharacter.h"

void UURPProjectileBase::Execute(AURPPlayerCharacter* Owner)
{
    if (!Owner || !Owner->HasAuthority()) return;

    FVector Loc = Owner->GetActorLocation() + Owner->GetActorRotation().RotateVector(SpawnOffset);
    FRotator Rot = Owner->GetActorRotation();

    FActorSpawnParameters Params;
    Params.Owner = Owner;

    AActor* Proj = Owner->GetWorld()->SpawnActor<AActor>(ProjectileClass, Loc, Rot, Params);

    // Projectile 안에서 OnHit 시 Monster->ApplyDamage(Owner->AttackPower * DamageMultiplier);
}