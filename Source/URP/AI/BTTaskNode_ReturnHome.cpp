// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_ReturnHome.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Types/URPCommonEnums.h"
#include "AIController.h"
#include "Characters/Monster/URPMonsterCharacter.h"

UBTTaskNode_ReturnHome::UBTTaskNode_ReturnHome()
{
    NodeName = "Return To Home";
}

EBTNodeResult::Type UBTTaskNode_ReturnHome::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*)
{
    AAIController* AI = OwnerComp.GetAIOwner();
    auto* BB = AI->GetBlackboardComponent();

    FVector Home = BB->GetValueAsVector("HomeLocation");
    AI->MoveToLocation(Home, 80.f);

    return EBTNodeResult::Succeeded;
}