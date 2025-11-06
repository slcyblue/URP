#include "URPUISubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void UURPUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    LoadScreenPaths();

    UE_LOG(LogTemp, Log, TEXT("[UIManager] Initialized and screen paths loaded."));
}

/**
 * @brief 경로 기반으로 UI 위젯 자동 로드
 * - /Game/UI/URPLoginWidget
 * - /Game/UI/URPCharacterSelectWidget
 * - /Game/UI/URPVillageHUD
 */
void UURPUISubsystem::LoadScreenPaths()
{
    const TMap<EURPScreenType, FString> PathMap = {
        { EURPScreenType::Login, TEXT("/Game/UI/URPLoginWidget.URPLoginWidget_C") },
        { EURPScreenType::CharacterSelect, TEXT("/Game/UI/URPCharacterSelectWidget.URPCharacterSelectWidget_C") },
        { EURPScreenType::VillageHUD, TEXT("/Game/UI/URPVillageHUD.URPVillageHUD_C") },
        { EURPScreenType::Loading, TEXT("/Game/UI/URPLoadingWidget.URPLoadingWidget_C") }
    };

    for (const auto& Pair : PathMap)
    {
        const FString& Path = Pair.Value;
        if (UClass* Loaded = LoadClass<UUserWidget>(nullptr, *Path))
        {
            ScreenMap.Add(Pair.Key, Loaded);
            UE_LOG(LogTemp, Log, TEXT("[UIManager] Loaded UI: %s"), *Path);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[UIManager] Failed to load UI: %s"), *Path);
        }
    }
}

/**
 * @brief 화면 교체
 */
void UURPUISubsystem::ShowScreen(EURPScreenType ScreenType)
{
    if (!GetWorld()) return;

    // 기존 UI 제거
    if (CurrentScreen)
    {
        CurrentScreen->RemoveFromParent();
        CurrentScreen = nullptr;
    }

    // 새 UI 생성
    if (TSubclassOf<UUserWidget>* FoundClass = ScreenMap.Find(ScreenType))
    {
        CurrentScreen = CreateWidget<UUserWidget>(GetWorld(), *FoundClass);
        if (CurrentScreen)
        {
            CurrentScreen->AddToViewport();
            UE_LOG(LogTemp, Log, TEXT("[UIManager] Switched to screen: %d"), (uint8)ScreenType);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[UIManager] Failed to create screen widget for type: %d"), (uint8)ScreenType);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[UIManager] ScreenType not found in map: %d"), (uint8)ScreenType);
    }
}

/**
 * @brief 현재 화면 숨기기
 */
void UURPUISubsystem::HideCurrentScreen()
{
    if (CurrentScreen)
    {
        CurrentScreen->RemoveFromParent();
        CurrentScreen = nullptr;
        UE_LOG(LogTemp, Log, TEXT("[UIManager] Current screen hidden."));
    }
}

/**
 * @brief 팝업 표시 (ZOrder 높은 순서)
 */
void UURPUISubsystem::ShowPopup(TSubclassOf<UUserWidget> PopupClass)
{
    if (!GetWorld() || !PopupClass) return;

    UUserWidget* Popup = CreateWidget<UUserWidget>(GetWorld(), PopupClass);
    if (Popup)
    {
        Popup->AddToViewport(1000);
        UE_LOG(LogTemp, Log, TEXT("[UIManager] Popup displayed."));
    }
}
