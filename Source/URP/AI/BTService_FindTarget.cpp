#include "BTService_FindTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/Monster/URPMonsterCharacter.h"
#include <Characters/Player/URPPlayerCharacter.h>

UBTService_FindTarget::UBTService_FindTarget()
{
    bNotifyBecomeRelevant = true;
    Interval = 0.5f;
}

void UBTService_FindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* AI = OwnerComp.GetAIOwner();
    if (!AI || !AI->HasAuthority()) return;

    APawn* AIPawn = AI->GetPawn();
    if (!AIPawn) return;

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return;

    // 상태 체크 (Idle/Patrol일 때만)
    EAIState State = (EAIState)BB->GetValueAsEnum("AIState");
    if (State != EAIState::Idle && State != EAIState::Patrol)
        return;

    BB->SetValueAsObject("TargetActor", nullptr);
    BB->SetValueAsBool("HasTarget", false);

    FVector Origin = AIPawn->GetActorLocation();

    TArray<FOverlapResult> Results;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(AIPawn);

    bool bHit = AIPawn->GetWorld()->OverlapMultiByObjectType(
        Results,
        Origin,
        FQuat::Identity,
        FCollisionObjectQueryParams(ECC_Pawn),
        FCollisionShape::MakeSphere(SearchRadius),
        QueryParams
    );

    if (!bHit) return;

    float BestDist = FLT_MAX;
    AURPPlayerCharacter* BestTarget = nullptr;

    for (auto& R : Results)
    {
        AURPPlayerCharacter* Candidate = Cast<AURPPlayerCharacter>(R.GetActor());
        if (!Candidate || Candidate == AIPawn || Candidate->bIsDead) continue;

        float Dist = FVector::Dist(Origin, Candidate->GetActorLocation());
        if (Dist < BestDist)
        {
            BestDist = Dist;
            BestTarget = Candidate;
        }
    }

    if (BestTarget)
    {
        BB->SetValueAsObject("TargetActor", BestTarget);
        BB->SetValueAsBool("HasTarget", true);

        if (AURPMonsterCharacter* M = Cast<AURPMonsterCharacter>(AIPawn))
        {
            M->SetTargetFromBlackboard(BestTarget);
        }
    }
}
