#include "URPCombatComponent.h"
#include "Characters/Player/URPPlayerCharacter.h"
#include "Characters/Common/URPClassComponent.h"
#include "Characters/Monster/URPMonsterCharacter.h"
#include "Core/GamePlay/Mechanics/Skill/System/URPSkillComponent.h"

UURPCombatComponent::UURPCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UURPCombatComponent::BeginPlay()
{
    Super::BeginPlay();
    OwnerPC = Cast<AURPPlayerCharacter>(GetOwner());
    ClassComp = OwnerPC ? OwnerPC->FindComponentByClass<UURPClassComponent>() : nullptr;
    SkillComp = OwnerPC ? OwnerPC->FindComponentByClass<UURPSkillComponent>() : nullptr;
}

void UURPCombatComponent::Attack()
{
    if (!OwnerPC)
        return;

    // 로컬 플레이어 → 서버 RPC
    if (OwnerPC->IsLocallyControlled())
    {
        ServerAttack();
    }

    // 공격 애니메이션은 로컬에서만
    OwnerPC->PlayAttack();
}

void UURPCombatComponent::ServerAttack_Implementation()
{
    if (!OwnerPC)
        return;

    // 서버에서 직접 HitTrace 실행
    PerformServerHitTrace();
}

void UURPCombatComponent::PerformServerHitTrace()
{
    FVector Start = OwnerPC->GetActorLocation() + FVector(0, 0, 50);
    FVector End = Start + OwnerPC->GetActorForwardVector() * 200.f;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwnerPC);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit, Start, End, ECC_Pawn, Params
    );

#if WITH_EDITOR
    // 디버그용 라인
    DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Red : FColor::Green, false, 0.3f, 0, 2.f);
#endif

    if (!bHit)
        return;

    AURPMonsterCharacter* Target = Cast<AURPMonsterCharacter>(Hit.GetActor());
    if (Target)
    {
        float Damage = GetFinalDamage();
        Target->ApplyDamage(Damage);
    }
}

float UURPCombatComponent::GetFinalDamage() const
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

void UURPCombatComponent::Skill(int32 SkillId)
{
    if (!OwnerPC || !SkillComp)
        return;

    // 1) 로컬 선쿨다운 체크
    if (SkillComp->GetRemainingCooldown(SkillId) > 0.f)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Client] Skill %d cooldown"), SkillId);
        return;
    }

    // 2) 서버 호출
    if (OwnerPC->IsLocallyControlled())
    {
        float ClientTime = GetWorld()->GetTimeSeconds();
        ServerSkill(SkillId, ClientTime);
    }

    OwnerPC->PlaySkill(); // 애니메이션은 로컬
}

void UURPCombatComponent::UseSkillSlot(int32 SlotIndex)
{
    if (!SkillComp || SlotIndex < 0)
        return;

    const TArray<int32>& Slots = SkillComp->GetSkillSlots();
    if (!Slots.IsValidIndex(SlotIndex))
        return;

    const int32 SkillId = Slots[SlotIndex];
    if (SkillId <= 0)
        return;

    Skill(SkillId); // 기존 Skill() 경로 재사용 (RPC + 애니메이션)
}

void UURPCombatComponent::ServerSkill_Implementation(int32 SkillId, float ClientTime)
{
    if (!SkillComp) return;

    const float ServerNow = GetWorld()->GetTimeSeconds();
    const float RTT = ServerNow - ClientTime;

    // 보정된 서버 기준 스킬 사용 시간
    float AdjustedServerTime = ServerNow - (RTT * 0.5f);

    SkillComp->ExecuteSkill(SkillId, AdjustedServerTime);
}