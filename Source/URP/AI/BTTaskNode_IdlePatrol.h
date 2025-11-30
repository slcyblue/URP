// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_IdlePatrol.generated.h"

/**
 * 
 */
UCLASS()
class URP_API UBTTaskNode_IdlePatrol : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTTaskNode_IdlePatrol();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*) override;

    UPROPERTY(EditAnywhere, Category = "AI")
    float PatrolRadius = 300.f;
};
