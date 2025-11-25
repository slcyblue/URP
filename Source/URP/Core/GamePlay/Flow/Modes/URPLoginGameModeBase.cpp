// Fill out your copyright notice in the Description page of Project Settings.


#include "URPLoginGameModeBase.h"
#include "UI/Login/URPLoginHUD.h"
#include "Core/GamePlay/Flow/Controllers/URPLoginPlayerController.h"

AURPLoginGameModeBase::AURPLoginGameModeBase()
{
    PlayerControllerClass = AURPLoginPlayerController::StaticClass();
    HUDClass = AURPLoginHUD::StaticClass();
}

void AURPLoginGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        UE_LOG(LogTemp, Log, TEXT("[LoginGameMode] Server started. Waiting for clients to connect."));
    }
}