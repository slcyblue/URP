// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_ReturnHome.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Types/URPCommonEnums.h"
#include "AIController.h"
#include "Characters/Monster/URPMonsterCharacter.h"

UBTTaskNode_ReturnHome::UBTTaskNode_ReturnHome()
{
    NodeName = "Return To Home";
}

EBTNodeResult::Type UBTTaskNode_ReturnHome::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*)
{
    auto* AI = OwnerComp.GetAIOwner();
    auto* BB = OwnerComp.GetBlackboardComponent();
    if (!AI || !AI->HasAuthority() || !BB) return EBTNodeResult::Failed;

    FVector Home = BB->GetValueAsVector("HomeLocation");
    APawn* Pawn = AI->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    const float Distance = FVector::Dist(Pawn->GetActorLocation(), Home);

    // Home에 거의 도착함 → Idle로 전환
    if (Distance < 60.f)
    {
        BB->SetValueAsEnum("AIState", (uint8)EAIState::Idle);
        BB->SetValueAsBool("HasTarget", false);
        BB->SetValueAsObject("TargetActor", nullptr);

        if (AURPMonsterCharacter* Monster = Cast<AURPMonsterCharacter>(Pawn))
        {
            Monster->ClearTarget();
        }

        return EBTNodeResult::Succeeded;
    }

    // 아직 복귀 중 → 이동 명령
    AI->MoveToLocation(Home);
    BB->SetValueAsEnum("AIState", (uint8)EAIState::Return);
    return EBTNodeResult::Succeeded;
}