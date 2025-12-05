#include "AI/BTDecorator_PatrolCooldown.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTDecorator_PatrolCooldown::UBTDecorator_PatrolCooldown()
{
    NodeName = TEXT("Patrol Cooldown");
    MinInterval = 2.0f;
    MaxInterval = 4.0f;
}

uint16 UBTDecorator_PatrolCooldown::GetInstanceMemorySize() const
{
    // float 하나(NextAllowedTime)를 인스턴스 메모리로 사용
    return sizeof(float);
}

bool UBTDecorator_PatrolCooldown::CalculateRawConditionValue(
    UBehaviorTreeComponent& OwnerComp,
    uint8* NodeMemory) const
{
    float& NextAllowedTime = *reinterpret_cast<float*>(NodeMemory);

    UWorld* World = OwnerComp.GetWorld();
    if (!World)
        return false;

    const float Now = World->GetTimeSeconds();

    // 아직 한 번도 사용된 적 없는 경우
    if (NextAllowedTime <= 0.f)
    {
        // 지금은 바로 통과시키고, 다음 허용 시간을 설정
        const float Interval = FMath::FRandRange(MinInterval, MaxInterval);
        NextAllowedTime = Now + Interval;
        return true;
    }

    // 쿨타임이 지난 경우 → 이번엔 허용 & 다음 시간 갱신
    if (Now >= NextAllowedTime)
    {
        const float Interval = FMath::FRandRange(MinInterval, MaxInterval);
        NextAllowedTime = Now + Interval;
        return true;
    }

    // 아직 쿨타임 안 지남 → 실행 막기
    return false;
}
