// Fill out your copyright notice in the Description page of Project Settings.


#include "URPLoginHUD.h"
#include "URPLoginWidget.h"
#include "Core/Subsystems/UI/URPUISubsystem.h"
#include "Blueprint/UserWidget.h"

const FString AURPLoginHUD::LoginWidgetPath = TEXT("/Game/UI/URPLoginWidget.URPLoginWidget");

AURPLoginHUD::AURPLoginHUD()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetFinder(TEXT("/Game/UI/URPLoginWidget"));
    if (WidgetFinder.Succeeded())
    {
        LoginWidgetClass = WidgetFinder.Class;
    }
}

void AURPLoginHUD::BeginPlay()
{
    Super::BeginPlay();

    if (UURPUISubsystem* UI = GetGameInstance()->GetSubsystem<UURPUISubsystem>())
    {
        UI->ShowScreen(EURPScreenType::Login);
    }
}