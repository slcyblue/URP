#include "AI/BTDecorator_IsReturnHome.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_IsReturnHome::CalculateRawConditionValue(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    auto* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return false;

    uint8 State = BB->GetValueAsEnum("AIState");

    return (State == (uint8)EAIState::Return);
}