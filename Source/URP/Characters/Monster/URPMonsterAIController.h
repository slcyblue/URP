#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "URPMonsterAIController.generated.h"

UCLASS()
class URP_API AURPMonsterAIController : public AAIController
{
    GENERATED_BODY()

public:
    AURPMonsterAIController();

    virtual void OnPossess(APawn* InPawn) override;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BehaviorTree;

    UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComp; }

private:
    UPROPERTY()
    UBlackboardComponent* BlackboardComp;
};
