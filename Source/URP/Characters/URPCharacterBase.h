// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/URPCommonEnums.h"
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
    /** 캐릭터 타입 (플레이어 클래스 타입 / 몬스터 타입 등) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "URP|Character")
    EURPClassType ClassType = EURPClassType::None;

    // 공통 스탯
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
    int64 MaxHp = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
    int64 CurrentHp = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
    float AttackPower = 10.f;

    // 공통 행동
    virtual void ApplyDamage(float Amount);

};
