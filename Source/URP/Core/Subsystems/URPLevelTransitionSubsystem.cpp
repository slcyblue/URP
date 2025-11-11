#include "URPLevelTransitionSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "Core/Subsystems/URPUISubsystem.h"
#include "TimerManager.h"

void UURPLevelTransitionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UURPLevelTransitionSubsystem::AsyncTransitionToLevel(FName LevelPackageName, EURPScreenType NextHUD)
{
    PendingLevelPackage = LevelPackageName;
    PendingHUD = NextHUD;
    CurrentProgress = 0.f;

    UGameplayStatics::OpenLevel(GetWorld(), LevelPackageName);

    //// 로딩 UI 표시
    //if (auto* UI = GetGameInstance()->GetSubsystem<UURPUISubsystem>())
    //{
    //    UI->ShowScreen(EURPScreenType::Loading);
    //    UI->UpdateLoadingProgress(0.f);
    //}

    //// 비동기 스트리밍 레벨 로드
    //FLatentActionInfo LatentInfo;
    //LatentInfo.CallbackTarget = this;
    //LatentInfo.ExecutionFunction = TEXT("OnStreamLevelLoaded");
    //LatentInfo.Linkage = 0;
    //LatentInfo.UUID = GetUniqueID(); // 유니크 ID 생성

    //UGameplayStatics::LoadStreamLevel(
    //    this->GetWorld(),
    //    LevelPackageName,
    //    true,    // makeVisibleAfterLoad
    //    true,    // shouldBlockOnLoad (false for non-blocking)
    //    LatentInfo
    //);

    //// 진행률 타이머 설정
    //GetWorld()->GetTimerManager().SetTimer(
    //    ProgressTimerHandle,
    //    this,
    //    &UURPLevelTransitionSubsystem::UpdateProgress,
    //    0.05f,
    //    true
    //);

    //UE_LOG(LogTemp, Log, TEXT("[LevelTransition] Async load started: %s"), *LevelPackageName.ToString());
}

void UURPLevelTransitionSubsystem::UpdateProgress()
{
    // 실제 엔진에서 스트리밍 진행률을 직접 제공하진 않음,
    // 대신 경험적으로 연출용 퍼센트 증가
    CurrentProgress = FMath::Clamp(CurrentProgress + 0.02f, 0.f, 0.9f);

    if (auto* UI = GetGameInstance()->GetSubsystem<UURPUISubsystem>())
    {
        UI->UpdateLoadingProgress(CurrentProgress);
    }
}

void UURPLevelTransitionSubsystem::OnStreamLevelLoaded()
{
    // 타이머 정지
    GetWorld()->GetTimerManager().ClearTimer(ProgressTimerHandle);

    // 완료 퍼센트 설정
    if (auto* UI = GetGameInstance()->GetSubsystem<UURPUISubsystem>())
    {
        UI->UpdateLoadingProgress(1.f);
    }

    UE_LOG(LogTemp, Log, TEXT("[LevelTransition] Level loaded: %s"), *PendingLevelPackage.ToString());

    // HUD 전환
    if (auto* UI = GetGameInstance()->GetSubsystem<UURPUISubsystem>())
    {
        UI->ShowScreen(PendingHUD);
    }
}
