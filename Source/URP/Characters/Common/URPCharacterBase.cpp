#include "Characters/Common/URPCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Core/GamePlay/Mechanics/Status/URPBuffDebuffComponent.h"
#include "Core/GamePlay/Mechanics/Skill/System/URPSkillComponent.h"
#include "Characters/Common/URPAnimInstance.h"
#include "Characters/Monster/URPMonsterCharacter.h"
#include "Characters/Player/URPPlayerCharacter.h"
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

    BuffDebuffComp = CreateDefaultSubobject<UURPBuffDebuffComponent>(TEXT("BuffDebuffComp"));
    SkillComponent = CreateDefaultSubobject<UURPSkillComponent>(TEXT("SkillComponent"));
    StatComponent = CreateDefaultSubobject<UURPStatComponent>(TEXT("StatComponent"));
}

// Called when the game starts or when spawned
void AURPCharacterBase::BeginPlay()
{
	Super::BeginPlay();

    AnimInstance = Cast<UURPAnimInstance>(GetMesh()->GetAnimInstance());
}

void AURPCharacterBase::ApplyDamage(float Amount)
{
    CurrentHp -= FMath::RoundToInt(Amount);
    CurrentHp = FMath::Clamp<int64>(CurrentHp, 0, StatComponent->GetFinalMaxHp());

    UE_LOG(LogTemp, Log, TEXT("[%s] Took Damage: %.1f | HP: %lld / %.1f"),
        *GetName(), Amount, CurrentHp, StatComponent->GetFinalMaxHp());

    if (CurrentHp <= 0 && !bIsDead)
    {
        bIsDead = true;

        if (HasAuthority())
        {
            OnCharacterDied.Broadcast(this);
        }

        if (AURPMonsterCharacter* Monster = Cast<AURPMonsterCharacter>(this))
        {
            Monster->Die();
        }
        else if (AURPPlayerCharacter* Player = Cast<AURPPlayerCharacter>(this))
        {
            Player->Die();
        }
    }
}

void AURPCharacterBase::RecalculateStats()
{
    if (StatComponent)
    {
        StatComponent->Recalculate();
        return;
    }
}

void AURPCharacterBase::HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    // 기본 동작 없음
}