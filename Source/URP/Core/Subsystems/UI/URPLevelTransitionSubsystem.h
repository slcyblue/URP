#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/LevelStreaming.h"
#include "Types/URPCommonEnums.h"
#include "URPLevelTransitionSubsystem.generated.h"

UCLASS()
class URP_API UURPLevelTransitionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    /** 비동기 레벨 로드 + 로딩 UI 표시 → 완전 전환 */
    UFUNCTION(BlueprintCallable)
    void AsyncTransitionToLevel(FName LevelPackageName, EURPScreenType NextHUD);

private:
    /** 로드가 완료됐을 때 실행 */
    UFUNCTION()
    void OnStreamLevelLoaded();

    /** 진행률 갱신 타이머 콜백 */
    void UpdateProgress();

private:
    FTimerHandle ProgressTimerHandle;
    float CurrentProgress = 0.f;
    EURPScreenType PendingHUD = EURPScreenType::None;
    FName PendingLevelPackage;
    ULevelStreaming* StreamingLevelInstance = nullptr;
};
