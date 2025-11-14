// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "URPMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class URP_API UURPMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float Speed = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsDead = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsAttacking = false;

public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
