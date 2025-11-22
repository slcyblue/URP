// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/URPPlayerData.h"
#include "URPPlayerCombatComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class URP_API UURPPlayerCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UURPPlayerCombatComponent();

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
    class UURPPlayerClassComponent* ClassComp;
    class UURPPlayerSkillComponent* SkillComp;

    void PerformServerHitTrace();

    float GetFinalDamage() const;

    bool bAttacking = false;
};