// Fill out your copyright notice in the Description page of Project Settings.

#include "URPMonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "URPMonsterCharacter.h"

AURPMonsterAIController::AURPMonsterAIController()
{
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
    BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComponent"));
}


void AURPMonsterAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (!InPawn || !BehaviorTreeAsset) return;

    // 서버 전용 AI
    if (!HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("[AI] Client tried to run AI — blocked."));
        return;
    }

    // Blackboard 초기화
    if (UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComp))
    {
        AURPMonsterCharacter* Monster = Cast<AURPMonsterCharacter>(InPawn);
        if (Monster)
        {
            const FVector SpawnLoc = Monster->GetActorLocation();

            BlackboardComp->SetValueAsVector("HomeLocation", SpawnLoc);
            BlackboardComp->SetValueAsBool("HasTarget", false);
            BlackboardComp->SetValueAsObject("TargetActor", nullptr);
            BlackboardComp->SetValueAsEnum("AIState", (uint8)EAIState::Idle);
            BlackboardComp->SetValueAsFloat("AttackCooldown", 0.f);
            BlackboardComp->SetValueAsVector("MoveDestination", SpawnLoc);
        }

        BTComponent->StartTree(*BehaviorTreeAsset);
    }
}

void AURPMonsterAIController::OnUnPossess()
{
    Super::OnUnPossess();

    if (BTComponent)
        BTComponent->StopTree();
}