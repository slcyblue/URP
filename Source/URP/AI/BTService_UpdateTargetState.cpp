// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_UpdateTargetState.h"
#include "AIController.h"
#include "Types/URPCommonEnums.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Monster/URPMonsterCharacter.h"

UBTService_UpdateTargetState::UBTService_UpdateTargetState()
{
    Interval = 0.2f;
}

void UBTService_UpdateTargetState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8*, float DeltaSeconds)
{
    AAIController* AI = OwnerComp.GetAIOwner();
    if (!AI || !AI->HasAuthority()) return;

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    APawn* Self = AI->GetPawn();
    if (!BB || !Self) return;

    AURPMonsterCharacter* Monster = Cast<AURPMonsterCharacter>(Self);
    if (!Monster) return;

    AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));

    // --- Target 없음 → Return ---
    if (!Target)
    {
        BB->SetValueAsEnum("AIState", (uint8)EAIState::Return);
        return;
    }

    float Dist = FVector::Dist(Self->GetActorLocation(), Target->GetActorLocation());
    float InAttackRange = Self->GetSimpleCollisionRadius() + 150.f;

    // --- Chase 중 제한 거리 초과 시 Return ---
    const float MaxChaseDistance = 1000.f;  // 원하는 값
    if (Dist > MaxChaseDistance)
    {
        Monster->ClearTarget(); // TargetActor = nullptr
        BB->SetValueAsEnum("AIState", (uint8)EAIState::Return);
        return;
    }

    // --- Attack / Chase 판정 ---
    if (Dist <= InAttackRange)
    {
        BB->SetValueAsBool("IsInAttackRange", true);
        BB->SetValueAsEnum("AIState", (uint8)EAIState::Attack);
    }
    else
    {
        BB->SetValueAsBool("IsInAttackRange", false);
        BB->SetValueAsEnum("AIState", (uint8)EAIState::Chase);
    }
}