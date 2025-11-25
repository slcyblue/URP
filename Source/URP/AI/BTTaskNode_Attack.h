// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Monster/URPMonsterCharacter.h"
#include "BTTaskNode_Attack.generated.h"

/**
 * 
 */
UCLASS()
class URP_API UBTTaskNode_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTTaskNode_Attack();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere)
    float AttackCooldownTime = 1.0f;
};
