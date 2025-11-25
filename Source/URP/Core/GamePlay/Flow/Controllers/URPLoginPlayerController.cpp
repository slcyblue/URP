// Fill out your copyright notice in the Description page of Project Settings.


#include "URPLoginPlayerController.h"

void AURPLoginPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalController())
    {
        bShowMouseCursor = true;
        SetInputMode(FInputModeUIOnly());
        UE_LOG(LogTemp, Log, TEXT("[LoginPC] Local controller ready, showing login UI."));
    }
}