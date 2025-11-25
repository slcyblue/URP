#include "URPProjectileBase.h"
#include "Characters/Monster/URPMonsterCharacter.h"

void UURPProjectileBase::Execute(AURPCharacterBase* Owner)
{
    OwnerPC = Owner;
    if (!ProjectileClass) return;

    FVector Loc = Owner->GetActorLocation() + SpawnOffset;
    FRotator Rot = Owner->GetActorRotation();

    Owner->GetWorld()->SpawnActor<AActor>(ProjectileClass, Loc, Rot);

    // Projectile 안에서 OnHit 시 Monster->ApplyDamage(Owner->AttackPower * DamageMultiplier);
}