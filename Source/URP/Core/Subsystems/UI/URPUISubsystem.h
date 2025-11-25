#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Types/URPCommonEnums.h"
#include "URPUISubsystem.generated.h"

/**
 * @brief 전체 화면 UI 교체 / 팝업 관리용 Subsystem
 * - Login / CharacterSelect / VillageHUD 등 교체
 * - 팝업창 중첩 표시 가능
 */

UCLASS()
class URP_API UURPUISubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    /** 화면 교체 (이전 화면 제거 후 새 UI 표시) */
    UFUNCTION(BlueprintCallable)
    void ShowScreen(EURPScreenType ScreenType);

    /** 현재 화면 숨기기 */
    UFUNCTION(BlueprintCallable)
    void HideCurrentScreen();

    /** 팝업 표시 (ZOrder 1000 이상) */
    UFUNCTION(BlueprintCallable)
    void ShowPopup(TSubclassOf<UUserWidget> PopupClass);

    /** 현재 표시 중인 화면 가져오기 */
    UFUNCTION(BlueprintPure)
    UUserWidget* GetCurrentScreen() const { return CurrentScreen; }

    UFUNCTION(BlueprintCallable)
    void UpdateLoadingProgress(float Progress);

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
    UPROPERTY()
    UUserWidget* CurrentScreen;

    /** 등록된 UI 클래스 */
    UPROPERTY()
    TMap<EURPScreenType, TSubclassOf<UUserWidget>> ScreenMap;

    UPROPERTY()
    UUserWidget* CurrentWidget;

    /** 자동 로드용 경로 매핑 */
    void LoadScreenPaths();
};
