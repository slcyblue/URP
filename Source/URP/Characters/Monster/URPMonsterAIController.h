// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Types/URPCommonEnums.h"
#include "URPMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class URP_API AURPMonsterAIController : public AAIController
{
	GENERATED_BODY()
	

public:
    AURPMonsterAIController();

    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;

    UBlackboardComponent* GetBB() const { return Blackboard; }

protected:
    /** 몬스터용 BT */
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

    /** Blackboard 컴포넌트 */
    UPROPERTY(VisibleAnywhere, Category = "AI")
    UBlackboardComponent* BlackboardComp;

    /** BT 실행용 컴포넌트 */
    UPROPERTY(VisibleAnywhere, Category = "AI")
    UBehaviorTreeComponent* BTComponent;
};
