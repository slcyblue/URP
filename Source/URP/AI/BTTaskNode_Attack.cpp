// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_Attack.h"
#include "Characters/Monster/URPMonsterAIController.h"

UBTTaskNode_Attack::UBTTaskNode_Attack()
{
    NodeName = "Attack Target";
}

EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*)
{
    AURPMonsterAIController* AI = Cast<AURPMonsterAIController>(OwnerComp.GetAIOwner());
    if (!AI) return EBTNodeResult::Failed;

    AURPMonsterCharacter* Monster = Cast<AURPMonsterCharacter>(AI->GetPawn());
    if (!Monster) return EBTNodeResult::Failed;

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return EBTNodeResult::Failed;

    AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));
    if (!Target) return EBTNodeResult::Failed;

    // ====== 실제 공격 ======
    Monster->PerformBasicAttack(Target);

    // ====== AttackSpeed 기반 다음 공격 시간 설정 ======
    float ASPD = Monster->StatComponent->GetFinalAttackSpeed();
    float Interval = 1.f / FMath::Max(ASPD, 0.1f);

    float Now = OwnerComp.GetWorld()->GetTimeSeconds();
    BB->SetValueAsFloat("NextAttackTime", Now + Interval);

    // 상태 유지
    BB->SetValueAsEnum("AIState", (uint8)EAIState::Attack);

    return EBTNodeResult::Succeeded;
}