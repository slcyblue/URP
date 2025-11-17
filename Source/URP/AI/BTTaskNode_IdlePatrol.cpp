#include "BTTaskNode_IdlePatrol.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

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

    // NavMesh에서 랜덤 위치 찾기
    FNavLocation RandomLoc;
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(AI->GetWorld());
    if (!NavSys) return EBTNodeResult::Failed;

    if (NavSys->GetRandomPointInNavigableRadius(Home, PatrolRadius, RandomLoc))
    {
        BB->SetValueAsVector("MoveDestination", RandomLoc.Location);
        AI->MoveToLocation(RandomLoc.Location, 5.f, true);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}