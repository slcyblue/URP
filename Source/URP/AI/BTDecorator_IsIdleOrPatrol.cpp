#include "AI/BTDecorator_IsIdleOrPatrol.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_IsIdleOrPatrol::CalculateRawConditionValue(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    auto* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return false;

    uint8 State = BB->GetValueAsEnum("AIState");

    return (State == (uint8)EAIState::Idle ||
        State == (uint8)EAIState::Patrol);
}