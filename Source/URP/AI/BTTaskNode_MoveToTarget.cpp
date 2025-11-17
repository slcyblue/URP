// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_MoveToTarget.h"

UBTTaskNode_MoveToTarget::UBTTaskNode_MoveToTarget()
{
    NodeName = "Move To Target";
}

EBTNodeResult::Type UBTTaskNode_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*)
{
    auto* AI = OwnerComp.GetAIOwner();
    if (!AI || !AI->HasAuthority()) return EBTNodeResult::Failed;

    auto* BB = OwnerComp.GetBlackboardComponent();
    AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
    if (!Target) return EBTNodeResult::Failed;

    AI->MoveToActor(Target, 5.f, true, true, true);
    return EBTNodeResult::Succeeded;
}