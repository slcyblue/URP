#include "BTService_FindTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/Monster/URPMonsterCharacter.h"

UBTService_FindTarget::UBTService_FindTarget()
{
    bNotifyBecomeRelevant = true;
    Interval = 0.5f;
}

void UBTService_FindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (!OwnerComp.GetAIOwner() || !OwnerComp.GetAIOwner()->HasAuthority()) return;

    APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (!AIPawn) return;

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return;

    FName TargetKey = "TargetActor";

    // 이미 타겟 있으면 패스
    if (BB->GetValueAsObject(TargetKey)) return;

    // 플레이어 검색
    AActor* ClosestTarget = nullptr;
    float BestDist = FLT_MAX;

    for (FConstPlayerControllerIterator It = AIPawn->GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (!PC || !PC->GetPawn()) continue;

        float Dist = FVector::Dist(AIPawn->GetActorLocation(), PC->GetPawn()->GetActorLocation());
        if (Dist < BestDist && Dist <= SearchRadius)
        {
            BestDist = Dist;
            ClosestTarget = PC->GetPawn();
        }
    }

    if (ClosestTarget)
    {
        BB->SetValueAsObject(TargetKey, ClosestTarget);
        BB->SetValueAsBool("HasTarget", true);

        if (AURPMonsterCharacter* Monster = Cast<AURPMonsterCharacter>(AIPawn))
        {
            if (IsValid(ClosestTarget))
            {
                Monster->SetTargetFromBlackboard(ClosestTarget);
            }
            else
            {
                Monster->ClearTarget();
            }
        }
    }
}
