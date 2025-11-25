// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/URPPlayerTypes.h"
#include "URPCombatComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class URP_API UURPCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UURPCombatComponent();

    void Attack();

    UFUNCTION(Server, Reliable)
    void ServerAttack();

    UFUNCTION(Server, Reliable)
    void ServerSkill(int32 SkillId, float ClientTime);

    void Skill(int32 SkillId);

protected:
    virtual void BeginPlay() override;

private:
    class AURPPlayerCharacter* OwnerPC;
    class UURPClassComponent* ClassComp;
    class UURPSkillComponent* SkillComp;

    void PerformServerHitTrace();

    float GetFinalDamage() const;

    bool bAttacking = false;
};