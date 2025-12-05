// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTDecorator_IsCooldownReady.generated.h"

/**
 * 
 */
UCLASS()
class URP_API UBTDecorator_IsCooldownReady : public UBTDecorator
{
    GENERATED_BODY()

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8*) const override;

};
