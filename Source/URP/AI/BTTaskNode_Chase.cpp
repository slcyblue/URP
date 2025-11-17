#include "BTTaskNode_Chase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/URPMonsterCharacter.h"

UBTTaskNode_Chase::UBTTaskNode_Chase()
{
    NodeName = "ChaseLogic (Check Lost Target)";
}

EBTNodeResult::Type UBTTaskNode_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*)
{
    AAIController* AI = OwnerComp.GetAIOwner();
    if (!AI || !AI->HasAuthority()) return EBTNodeResult::Failed;

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

    AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
    AURPMonsterCharacter* Monster = Cast<AURPMonsterCharacter>(AI->GetPawn());

    if (!Monster) return EBTNodeResult::Failed;

    // 타겟이 없으면 ReturnHome 상태로 전환
    if (!Target)
    {
        BB->SetValueAsEnum("AIState", (uint8)EAIState::Return);
        return EBTNodeResult::Succeeded;
    }

    // 타겟이 있으면 Chase 유지
    BB->SetValueAsEnum("AIState", (uint8)EAIState::Chase);
    return EBTNodeResult::Succeeded;
}
