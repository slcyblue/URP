// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsCooldownReady.h"

bool UBTDecorator_IsCooldownReady::CalculateRawConditionValue(
    UBehaviorTreeComponent& OwnerComp, uint8*) const
{
    auto* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return false;

    float Now = OwnerComp.GetWorld()->GetTimeSeconds();
    float NextAttackTime = BB->GetValueAsFloat("NextAttackTime");

    return (Now >= NextAttackTime);
}