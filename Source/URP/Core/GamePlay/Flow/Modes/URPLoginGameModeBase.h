// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "URPLoginGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class URP_API AURPLoginGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AURPLoginGameModeBase();

protected:
	virtual void BeginPlay() override;
};
