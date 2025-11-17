// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Chase.generated.h"

/**
 * 
 */
UCLASS()
class URP_API UBTTaskNode_Chase : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_Chase();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*) override;
};
