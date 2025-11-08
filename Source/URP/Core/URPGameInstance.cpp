// Fill out your copyright notice in the Description page of Project Settings.


#include "URPGameInstance.h"
#include "Subsystems/URPNetworkSubsystem.h"
#include "Subsystems/URPLevelTransitionSubsystem.h"
#include "Managers/URPUserDataManager.h"
#include "Managers/URPGameDataManager.h"
#include "Subsystems/URPUISubsystem.h"

void UURPGameInstance::Init()
{
    Super::Init();

    UE_LOG(LogTemp, Log, TEXT("[GI] Init"));

    // (자동) 네트워크 서브시스템 기동 확인
    GetSubsystem<UURPNetworkSubsystem>();
    GetSubsystem<UURPUISubsystem>();
    GetSubsystem<UURPLevelTransitionSubsystem>();

    // (수동) 싱글톤 매니저들 기동
    UURPUserDataManager::Get();     // 세션 동안 유지되는 유저런타임 데이터
    UURPGameDataManager::Get()->Initialize(); // 로컬 기본 로드 + 서버 버전 체크
}


void UURPGameInstance::Shutdown()
{
    Super::Shutdown();
    // 필요 시 싱글톤 정리 로직 추가
}