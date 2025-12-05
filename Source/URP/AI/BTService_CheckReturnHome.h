#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckReturnHome.generated.h"

UCLASS()
class URP_API UBTService_CheckReturnHome : public UBTService
{
    GENERATED_BODY()

public:
    UBTService_CheckReturnHome();

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};