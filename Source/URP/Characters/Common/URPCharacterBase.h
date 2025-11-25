// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Types/URPCommonEnums.h"
#include "Core/GamePlay/Mechanics/Skill/System/URPSkillComponent.h"
#include "Core/GamePlay/Mechanics/Status/URPBuffDebuffComponent.h"
#include "URPStatComponent.h"
#include "URPCharacterBase.generated.h"

UCLASS()
class URP_API AURPCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AURPCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
    // 기본 스탯(클래스에 의해 결정)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
    int64 BaseMaxHp = 100;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
    float BaseAttack = 10.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
    float BaseDefense = 5.f;


    // 장비 스탯(EquipmentComponent가 제공)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
    int64 EquipMaxHp = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
    float EquipAttack = 0.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
    float EquipDefense = 0.f;


    // 최종 스탯 (Base + Equip + Buff 등)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
    int64 MaxHp = 100;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
    float AttackPower = 10.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
    float DefensePower = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
    int64 CurrentHp = 100;

    UPROPERTY(VisibleAnywhere)
    UURPBuffDebuffComponent* BuffDebuffComp;

    UPROPERTY(VisibleAnywhere)
    UURPSkillComponent* SkillComponent;

    UPROPERTY(VisibleAnywhere)
    UURPStatComponent* StatComponent;

    // 공통 행동
    virtual void ApplyDamage(float Amount);

    UFUNCTION(BlueprintCallable)
    virtual void RecalculateStats();

};
