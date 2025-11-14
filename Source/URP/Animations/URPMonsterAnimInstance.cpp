#include "Animations/URPMonsterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UURPMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    ACharacter* Owner = Cast<ACharacter>(TryGetPawnOwner());
    if (!Owner) return;

    Speed = Owner->GetVelocity().Size();
}