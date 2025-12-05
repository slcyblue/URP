#include "BTService_CheckReturnHome.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "URPCommonEnums.h"

UBTService_CheckReturnHome::UBTService_CheckReturnHome()
{
    bNotifyTick = true;
}

void UBTService_CheckReturnHome::TickNode(
    UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
    float DeltaSeconds)
{
    AAIController* AI = OwnerComp.GetAIOwner();
    if (!AI) return;

    UBlackboardComponent* BB = AI->GetBlackboardComponent();
    APawn* Pawn = AI->GetPawn();
    if (!BB || !Pawn) return;

    uint8 State = BB->GetValueAsEnum("AIState");
    if (State != (uint8)EAIState::Return)
        return;

    FVector Home = BB->GetValueAsVector("HomeLocation");
    float Dist = FVector::Dist(Pawn->GetActorLocation(), Home);

    if (Dist < 80.f)
    {
        BB->SetValueAsEnum("AIState", (uint8)EAIState::Idle);
        BB->SetValueAsObject("TargetActor", nullptr);
    }
}
