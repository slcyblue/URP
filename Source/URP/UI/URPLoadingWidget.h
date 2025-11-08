// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "TimerManager.h"
#include "URPLoadingWidget.generated.h"

/**
 * 
 */
UCLASS()
class URP_API UURPLoadingWidget : public UUserWidget
{
	GENERATED_BODY()


public:
    /** C++에서 수동으로 생성할 때 초기화 */
    virtual void NativeConstruct() override;

    /** ProgressBar 갱신 */
    UFUNCTION()
    void SetProgress(float InPercent);

    /** Fade 연출 (텍스트 점 1~3개 순환) */
    UFUNCTION()
    void UpdateFadeText();

protected:
    /** 프로그래스 바 */
    UPROPERTY(meta = (BindWidgetOptional))
    UProgressBar* ProgressBar;

    /** "Loading..." 텍스트 */
    UPROPERTY(meta = (BindWidgetOptional))
    UTextBlock* LoadingText;

private:
    FTimerHandle FadeTimer;
    int32 DotCount = 0;
};
