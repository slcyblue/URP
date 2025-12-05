#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_PatrolCooldown.generated.h"

/**
 * 일정 주기(쿨타임)마다만 Patrol을 허용하는 데코레이터
 * - Idle/Patrol 브랜치 / IdlePatrol Task 옆에 붙여서 사용
 */
UCLASS()
class URP_API UBTDecorator_PatrolCooldown : public UBTDecorator
{
    GENERATED_BODY()

public:
    UBTDecorator_PatrolCooldown();

    // 최소 / 최대 주기 (초)
    UPROPERTY(EditAnywhere, Category = "Cooldown")
    float MinInterval;

    UPROPERTY(EditAnywhere, Category = "Cooldown")
    float MaxInterval;

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
        uint8* NodeMemory) const override;

    virtual uint16 GetInstanceMemorySize() const override;
};
