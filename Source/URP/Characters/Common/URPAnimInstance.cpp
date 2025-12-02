#include "URPAnimInstance.h"
#include "Characters/Common/URPCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UURPAnimInstance::UURPAnimInstance()
{
}

void UURPAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    OwnerCharacter = Cast<AURPCharacterBase>(TryGetPawnOwner());
    OnMontageEnded.AddDynamic(this, &UURPAnimInstance::IsMontageEnded);
}

void UURPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!OwnerCharacter.IsValid())
        OwnerCharacter = Cast<AURPCharacterBase>(TryGetPawnOwner());

    if (!OwnerCharacter.IsValid()) return;

    // 이동 속도 계산
    Speed = OwnerCharacter->GetVelocity().Size();

    // 캐릭터가 실제로 움직이고 있는지 판정
    bIsMoving = Speed > 5.f;

    // 매 틱 공격속도 가져오기
    CachedAttackSpeed = OwnerCharacter->StatComponent->GetFinalAttackSpeed();
}

float UURPAnimInstance::PlayNormalMontage(UAnimMontage* Montage)
{
    if (!Montage || !OwnerCharacter.IsValid()) return 0.f;

    return Montage_Play(Montage, CachedAttackSpeed);
}

float UURPAnimInstance::PlayCombatMontage(UAnimMontage* Montage, float AnimSpeed)
{
    if (!Montage || !OwnerCharacter.IsValid()) return 0.f;

    return Montage_Play(Montage, AnimSpeed * CachedAttackSpeed);
}

void UURPAnimInstance::StopAllMontages(float BlendOutTime)
{
    Montage_Stop(BlendOutTime);
}

void UURPAnimInstance::IsMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    AURPCharacterBase* Owner = Cast<AURPCharacterBase>(TryGetPawnOwner());
    if (!Owner) return;

    Owner->HandleMontageEnded(Montage, bInterrupted);
}