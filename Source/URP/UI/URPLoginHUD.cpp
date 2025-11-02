// Fill out your copyright notice in the Description page of Project Settings.


#include "URPLoginHUD.h"
#include "URPLoginWidget.h"
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
///Game/UI/SampleWidget.SampleWidget

void AURPLoginHUD::BeginPlay()
{
    Super::BeginPlay();

    /*if (APlayerController* PC = GetOwningPlayerController())
    {
        if (PC->IsLocalController())
        {
            if (LoginWidgetClass)
            {
                LoginWidgetInstance = CreateWidget<UURPLoginWidget>(GetWorld(), LoginWidgetClass);
                if (LoginWidgetInstance)
                {
                    LoginWidgetInstance->AddToViewport();
                    UE_LOG(LogTemp, Log, TEXT("[LoginHUD] Login UI added to viewport."));
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("[LoginHUD] LoginWidgetClass is NULL."));
            }
        }
    }*/

    //if (IsValid(GetWorld()) && GetOwningPlayerController() && GetOwningPlayerController()->IsLocalController())
    //{
    //    //직접 경로 기반으로 UClass 로드
    //    UClass* WidgetClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *LoginWidgetPath));

    //    if (WidgetClass)
    //    {
    //        LoginWidgetInstance = CreateWidget<UURPLoginWidget>(GetWorld(), WidgetClass);
    //        if (LoginWidgetInstance)
    //        {
    //            LoginWidgetInstance->AddToViewport();
    //            UE_LOG(LogTemp, Log, TEXT("[LoginHUD] LoginWidget loaded from code path: %s"), *LoginWidgetPath);
    //        }
    //    }
    //    else
    //    {
    //        UE_LOG(LogTemp, Warning, TEXT("[LoginHUD] Failed to load widget class at path: %s"), *LoginWidgetPath);
    //    }
    //}

    if (IsValid(LoginWidgetClass))
    {
        auto* Widget = CreateWidget<UUserWidget>(GetWorld(), LoginWidgetClass);
        Widget->AddToViewport();
    }
}