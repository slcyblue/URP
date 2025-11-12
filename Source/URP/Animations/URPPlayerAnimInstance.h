#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "URPPlayerAnimInstance.generated.h"

UCLASS()
class URP_API UURPPlayerAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    bool bIsMoving = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    bool bIsAttacking = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
    bool bIsUsingSkill = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Speed = 0.f;

public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    /** 외부에서 상태 변경할 때 호출 */
    void SetIsMoving(bool bValue) { bIsMoving = bValue; }
    void SetIsAttacking(bool bValue) { bIsAttacking = bValue; }
    void SetIsUsingSkill(bool bValue) { bIsUsingSkill = bValue; }
};
