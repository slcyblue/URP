// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTDecorator_IsInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class URP_API UBTDecorator_IsInAttackRange : public UBTDecorator
{
	GENERATED_BODY()

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8*) const override
    {
        auto* BB = OwnerComp.GetBlackboardComponent();
        return BB->GetValueAsBool("IsInAttackRange");
    }
};
