#include "URPPlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

void UURPPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APawn* OwnerPawn = TryGetPawnOwner();
    if (!OwnerPawn) return;

    // 이동 속도 계산
    Speed = OwnerPawn->GetVelocity().Size();

    // 캐릭터가 실제로 움직이고 있는지 판정
    bIsMoving = Speed > 5.f;
}
