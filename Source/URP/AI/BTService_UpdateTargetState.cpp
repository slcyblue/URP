// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_UpdateTargetState.h"
#include "AIController.h"
#include "Types/URPCommonEnums.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateTargetState::UBTService_UpdateTargetState()
{
    Interval = 0.2f;
}

void UBTService_UpdateTargetState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8*, float DeltaSeconds)
{
    auto* AI = OwnerComp.GetAIOwner();
    if (!AI || !AI->HasAuthority()) return;

    auto* BB = OwnerComp.GetBlackboardComponent();
    APawn* SelfPawn = AI->GetPawn();
    if (!SelfPawn || !BB) return;

    AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));

    // 쿨다운 업데이트
    float Cooldown = BB->GetValueAsFloat("AttackCooldown");
    BB->SetValueAsFloat("AttackCooldown", FMath::Max(Cooldown - DeltaSeconds, 0.0f));

    if (!Target)
    {
        BB->SetValueAsBool("IsInAttackRange", false);
        return;
    }

    // 거리 기반 Attack / Chase 상태만 계속 업데이트
    float Dist = FVector::Dist(SelfPawn->GetActorLocation(), Target->GetActorLocation());
    BB->SetValueAsBool("IsInAttackRange", Dist <= AttackRange);
    BB->SetValueAsVector("LastSeenLocation", Target->GetActorLocation());

    if (Dist > AttackRange)
        BB->SetValueAsEnum("AIState", (uint8)EAIState::Chase);
    else
        BB->SetValueAsEnum("AIState", (uint8)EAIState::Attack);
}