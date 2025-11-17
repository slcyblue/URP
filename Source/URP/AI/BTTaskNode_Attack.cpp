// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_Attack.h"

UBTTaskNode_Attack::UBTTaskNode_Attack()
{
    NodeName = "Attack Target";
}

EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*)
{
    AAIController* AI = OwnerComp.GetAIOwner();
    if (!AI || !AI->HasAuthority()) return EBTNodeResult::Failed;

    auto* BB = OwnerComp.GetBlackboardComponent();
    auto* Monster = Cast<AURPMonsterCharacter>(AI->GetPawn());
    AActor* Target = Cast<AActor>(BB->GetValueAsObject("TargetActor"));

    if (!Monster || !Target) return EBTNodeResult::Failed;

    //// 공격 애니메이션 실행
    //Monster->PlayAttackMontage();   // 네 함수에 맞게 수정!

    //// 데미지 주기
    //Monster->DealDamageTo(Target);

    // 쿨다운 초기화
    BB->SetValueAsFloat("AttackCooldown", AttackCooldownTime);

    return EBTNodeResult::Succeeded;
}