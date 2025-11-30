#include "URPMonsterAIController.h"
#include "Types/URPCommonEnums.h"

AURPMonsterAIController::AURPMonsterAIController()
{
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void AURPMonsterAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (!BehaviorTree) return;

    UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComp);

    // 여기서는 HomeLocation/BT 실행 절대 하지 않음
    BlackboardComp->SetValueAsEnum("AIState", (uint8)EAIState::Idle);
    BlackboardComp->SetValueAsBool("HasTarget", false);
}
