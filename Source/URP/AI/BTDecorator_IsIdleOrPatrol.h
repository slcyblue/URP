#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Types/URPCommonEnums.h"
#include "BTDecorator_IsIdleOrPatrol.generated.h"


UCLASS()
class URP_API UBTDecorator_IsIdleOrPatrol : public UBTDecorator
{
	GENERATED_BODY()
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
