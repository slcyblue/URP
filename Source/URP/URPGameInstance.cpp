// Fill out your copyright notice in the Description page of Project Settings.


#include "URPGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UURPGameInstance::Init()
{
    Super::Init();

    // Listen 서버용 NetworkManager 스폰
    UWorld* World = GetWorld();
    if (World)
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        NetworkManager = World->SpawnActor<AURPNetworkManager>(AURPNetworkManager::StaticClass(), FTransform::Identity, Params);
        UE_LOG(LogTemp, Log, TEXT("[GameInstance] URPNetworkManager initialized."));
    }

    // 로그인 레벨로 전환
    const FString LoginLevel = TEXT("Login");
    if (UGameplayStatics::GetCurrentLevelName(GetWorld()) != LoginLevel)
    {
        UGameplayStatics::OpenLevel(GetWorld(), FName(*LoginLevel));
        UE_LOG(LogTemp, Log, TEXT("[GameInstance] Loading LoginLevel (ListenServer)."));
    }

}