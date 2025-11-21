// Fill out your copyright notice in the Description page of Project Settings.


#include "URP/Characters/URPCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Characters/URPMonsterCharacter.h"
#include "Characters/URPPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AURPCharacterBase::AURPCharacterBase()
{
    // 캡슐 기본 설정
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

    // 이동 설정 (공통)
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
    GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

// Called when the game starts or when spawned
void AURPCharacterBase::BeginPlay()
{
	Super::BeginPlay();

    CurrentHp = MaxHp;
	
}

void AURPCharacterBase::ApplyDamage(float Amount)
{
    CurrentHp -= FMath::RoundToInt(Amount);
    CurrentHp = FMath::Clamp<int64>(CurrentHp, 0, MaxHp);

    UE_LOG(LogTemp, Log, TEXT("[%s] Took Damage: %.1f | HP: %lld / %lld"),
        *GetName(), Amount, CurrentHp, MaxHp);

    if (CurrentHp <= 0)
    {
        if (AURPMonsterCharacter* Monster = Cast<AURPMonsterCharacter>(this))
        {
            Monster->Die();
        }
        else
        {
            // 플레이어 죽음은 나중에 구현
        }
    }
}

void AURPCharacterBase::RecalculateStats()
{
    MaxHp = BaseMaxHp + EquipMaxHp;
    AttackPower = BaseAttack + EquipAttack;
    DefensePower = BaseDefense + EquipDefense;

    // HP가 넘어지지 않도록 보정
    if (CurrentHp > MaxHp)
        CurrentHp = MaxHp;

    UE_LOG(LogTemp, Log, TEXT("[Recalc Stat] MaxHp=%lld, Attack=%.1f, Def=%.1f"),
        MaxHp, AttackPower, DefensePower);
}