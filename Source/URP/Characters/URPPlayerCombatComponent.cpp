#include "URPPlayerCombatComponent.h"
#include "URPPlayerCharacter.h"
#include "URPPlayerClassComponent.h"
#include "URPMonsterCharacter.h"
#include "URPPlayerSkillComponent.h"

UURPPlayerCombatComponent::UURPPlayerCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UURPPlayerCombatComponent::BeginPlay()
{
    Super::BeginPlay();
    OwnerPC = Cast<AURPPlayerCharacter>(GetOwner());
    ClassComp = OwnerPC ? OwnerPC->FindComponentByClass<UURPPlayerClassComponent>() : nullptr;
    SkillComp = OwnerPC ? OwnerPC->FindComponentByClass<UURPPlayerSkillComponent>() : nullptr;
}

void UURPPlayerCombatComponent::Attack()
{
    if (!OwnerPC)
        return;

    // 로컬 플레이어 → 서버 RPC
    if (OwnerPC->IsLocallyControlled())
    {
        ServerAttack();
    }

    // 공격 애니메이션은 로컬에서만 (모든 클라에서 자연스럽게 보임)
    OwnerPC->PlayAttack();
}

void UURPPlayerCombatComponent::ServerAttack_Implementation()
{
    if (!OwnerPC)
        return;

    // 서버에서 직접 HitTrace 실행
    PerformServerHitTrace();
}

void UURPPlayerCombatComponent::PerformServerHitTrace()
{
    FVector Start = OwnerPC->GetActorLocation() + FVector(0, 0, 50);
    FVector End = Start + OwnerPC->GetActorForwardVector() * 200.f;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwnerPC);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit, Start, End, ECC_Pawn, Params
    );

    if (!bHit)
        return;

    if (AURPMonsterCharacter* Monster = Cast<AURPMonsterCharacter>(Hit.GetActor()))
    {
        float Damage = GetFinalDamage();
        Monster->ApplyDamage(Damage);
    }
}

float UURPPlayerCombatComponent::GetFinalDamage() const
{
    if (!OwnerPC) return 0.f;

    float BaseDamage = OwnerPC->AttackPower;

    // 직업별 보정도 가능
    if (ClassComp && ClassComp->GetClassData()->ClassType == EURPClassType::Rogue)
    {
        // 예: 뒷치기 보정
        // BaseDamage *= 1.5f;
    }

    return BaseDamage;
}

void UURPPlayerCombatComponent::Skill(int32 SkillId)
{
    // 로컬 플레이어 → 서버 호출
    if (OwnerPC && OwnerPC->IsLocallyControlled()) {
        float ClientTime = GetWorld()->GetTimeSeconds();
        ServerSkill(SkillId, ClientTime);
    }

    OwnerPC->PlaySkill(); // 애니메이션은 로컬
}

void UURPPlayerCombatComponent::ServerSkill_Implementation(int32 SkillId, float ClientTime)
{
    if (!SkillComp) return;

    const float ServerNow = GetWorld()->GetTimeSeconds();
    const float RTT = ServerNow - ClientTime;

    // 보정된 서버 기준 스킬 사용 시간
    float AdjustedServerTime = ServerNow - (RTT * 0.5f);

    SkillComp->ExecuteSkill(SkillId, AdjustedServerTime);
}