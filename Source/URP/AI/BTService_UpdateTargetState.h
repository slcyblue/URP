// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateTargetState.generated.h"

/**
 * 
 */
UCLASS()
class URP_API UBTService_UpdateTargetState : public UBTService
{
	GENERATED_BODY()

public:
    UBTService_UpdateTargetState();

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8*, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere)
    float AttackRange = 180.f;   // 몬스터 데이터 기반으로 바꿔도 됨
};
