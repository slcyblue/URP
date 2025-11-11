// Fill out your copyright notice in the Description page of Project Settings.

#include "URPVillageGameModeBase.h"
#include "Core/Subsystems/URPGameDataSubsystem.h"
#include "Core/Subsystems/URPUserDataSubsystem.h"
#include "Characters/URPPlayerController.h"
#include "Characters/URPPlayerCharacter.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

AURPVillageGameModeBase::AURPVillageGameModeBase()
{
    // 1. PlayerController 설정
    PlayerControllerClass = AURPPlayerController::StaticClass();

    // 2. Default Pawn (또는 PlayerCharacter) 설정
    DefaultPawnClass = nullptr;

    // 3. 필요시 HUD, GameState, PlayerState 도 설정 가능
    // HUDClass = AURPHUD::StaticClass();
    // GameStateClass = AURPGameState::StaticClass();

    UE_LOG(LogTemp, Log, TEXT("[VillageGameMode] Controller = %s"),
        *PlayerControllerClass->GetName());
}

void AURPVillageGameModeBase::BeginPlay() 
{
    Super::BeginPlay();

    InitFromCachedPlayerData();
}

void AURPVillageGameModeBase::InitFromCachedPlayerData()
{
    auto* User = GetGameInstance()->GetSubsystem<UURPUserDataSubsystem>();
    if (!User) { UE_LOG(LogTemp, Error, TEXT("[VillageGM] No UserDataSubsystem")); return; }

    const FPlayerData& Data = User->GetPlayerData();
    if (Data.PlayerId.IsEmpty()) { UE_LOG(LogTemp, Warning, TEXT("[VillageGM] Empty PlayerData")); return; }

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC) { UE_LOG(LogTemp, Error, TEXT("[VillageGM] No PlayerController")); return; }

    SpawnFromDataAsync(PC, Data);
}

void AURPVillageGameModeBase::SpawnFromDataAsync(APlayerController* PC, const FPlayerData& Data)
{
    auto* GDM = GetGameInstance()->GetSubsystem<UURPGameDataSubsystem>();
    if (!GDM) { UE_LOG(LogTemp, Error, TEXT("[VillageGM] No GameDataSubsystem")); return; }

    const TOptional<FString> PathOpt = GDM->GetPawnPathByClass(Data.SelectedClass);
    if (!PathOpt.IsSet())
    {
        UE_LOG(LogTemp, Error, TEXT("[VillageGM] Pawn path not found for class=%d"), (uint8)Data.SelectedClass);
        return;
    }

    const FSoftObjectPath SoftPath(*PathOpt.GetValue());
    if (!SoftPath.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("[VillageGM] Invalid soft path: %s"), *PathOpt.GetValue());
        return;
    }

    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

    // 비동기 로드 → 완료 콜백에서 스폰
    Streamable.RequestAsyncLoad(
        SoftPath,
        FStreamableDelegate::CreateUObject(this, &AURPVillageGameModeBase::OnPawnClassLoaded, SoftPath, PC, Data)
    );
}

void AURPVillageGameModeBase::OnPawnClassLoaded(FSoftObjectPath SoftPath, APlayerController* PC, FPlayerData Data)
{
    UClass* Loaded = Cast<UClass>(SoftPath.ResolveObject());
    if (!Loaded)
    {
        // 혹시 ResolveObject가 null이면 ToSoftObjectPath 경로에서 로드된 Obj를 다시 꺼내기
        Loaded = Cast<UClass>(SoftPath.TryLoad());
    }
    TSubclassOf<APawn> PawnClass = Loaded;

    if (!PC || !PawnClass)
    {
        UE_LOG(LogTemp, Error, TEXT("[VillageGM] Load failed: %s"), *SoftPath.ToString());
        return;
    }

    // 기존 Pawn 정리
    if (APawn* Existing = PC->GetPawn())
    {
        if (Existing->GetClass() != PawnClass)
            Existing->Destroy();
    }

    // 스폰 위치
    AActor* StartSpot = FindPlayerStart(PC);
    const FTransform TM = StartSpot ? StartSpot->GetActorTransform() : FTransform(FRotator::ZeroRotator, FVector::ZeroVector);

    // 스폰 & 포제스
    APawn* NewPawn = GetWorld()->SpawnActor<APawn>(PawnClass, TM);
    if (!NewPawn) { UE_LOG(LogTemp, Error, TEXT("[VillageGM] Spawn failed")); return; }
    PC->Possess(NewPawn);

    // TODO: 여기서 Data(레벨/스킬/인벤토리) 주입 (인터페이스/세터)
    UE_LOG(LogTemp, Log, TEXT("[VillageGM] Spawned %s for %s"), *PawnClass->GetName(), *Data.PlayerId);
}