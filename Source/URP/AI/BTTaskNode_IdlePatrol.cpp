#include "BTTaskNode_IdlePatrol.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include <URPCommonEnums.h>

UBTTaskNode_IdlePatrol::UBTTaskNode_IdlePatrol()
{
    NodeName = "Idle / Random Patrol";
}

EBTNodeResult::Type UBTTaskNode_IdlePatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*)
{
    AAIController* AI = OwnerComp.GetAIOwner();
    if (!AI || !AI->HasAuthority()) return EBTNodeResult::Failed;

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return EBTNodeResult::Failed;

    const FVector Home = BB->GetValueAsVector("HomeLocation");

    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(AI->GetWorld());
    if (!NavSys) return EBTNodeResult::Failed;

    FNavLocation RandomLoc;
    if (NavSys->GetRandomPointInNavigableRadius(Home, PatrolRadius, RandomLoc))
    {
        BB->SetValueAsVector("MoveDestination", RandomLoc.Location);
        BB->SetValueAsEnum("AIState", (uint8)EAIState::Patrol);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}