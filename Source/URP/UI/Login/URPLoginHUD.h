// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "URPLoginHUD.generated.h"

/**
 * 
 */
UCLASS()
class URP_API AURPLoginHUD : public AHUD
{
	GENERATED_BODY()
	
public:
    AURPLoginHUD();

public:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UURPLoginWidget> LoginWidgetClass;

private:
    UPROPERTY()
    class UURPLoginWidget* LoginWidgetInstance;

    //코드로 직접 로드할 UMG 경로 (에디터에서 지정하지 않아도 됨)
    static const FString LoginWidgetPath;
};
