// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/URPLoadingWidget.h"

void UURPLoadingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 초기화
    if (ProgressBar)
        ProgressBar->SetPercent(0.f);

    if (LoadingText)
        LoadingText->SetText(FText::FromString(TEXT("Loading")));

    // "Loading..." 텍스트 점 순환 애니메이션
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            FadeTimer, this, &UURPLoadingWidget::UpdateFadeText, 0.5f, true);
    }
}

void UURPLoadingWidget::SetProgress(float InPercent)
{
    if (ProgressBar)
    {
        ProgressBar->SetPercent(InPercent);
    }

    // 100% 도달 시 텍스트 교체
    if (InPercent >= 1.0f && LoadingText)
    {
        LoadingText->SetText(FText::FromString(TEXT("Complete!")));
    }
}

void UURPLoadingWidget::UpdateFadeText()
{
    if (!LoadingText) return;

    DotCount = (DotCount + 1) % 4; // 0~3
    FString Dots;
    for (int32 i = 0; i < DotCount; ++i)
    {
        Dots += TEXT(".");
    }
    LoadingText->SetText(FText::FromString(FString::Printf(TEXT("Loading%s"), *Dots)));
}
