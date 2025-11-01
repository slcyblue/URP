// Fill out your copyright notice in the Description page of Project Settings.


#include "URPGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UURPGameInstance::Init()
{
    Super::Init();

    UWorld* World = GetWorld();
    if (!World) return;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    NetworkManager = World->SpawnActor<AURPNetworkManager>(AURPNetworkManager::StaticClass(), FTransform::Identity, Params);

    UE_LOG(LogTemp, Log, TEXT("[GameInstance] URPNetworkManager initialized."));
}