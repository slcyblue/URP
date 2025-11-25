// Fill out your copyright notice in the Description page of Project Settings.


#include "URPGameInstance.h"
#include "Subsystems/Network/URPNetworkSubsystem.h"
#include "Subsystems/UI/URPLevelTransitionSubsystem.h"
#include "Subsystems/UI/URPUISubsystem.h"
#include "Subsystems/Data/URPGameDataSubsystem.h"
#include "Subsystems/Data/URPUserDataSubsystem.h"

void UURPGameInstance::Init()
{
    Super::Init();

    UE_LOG(LogTemp, Log, TEXT("[GI] Init"));

    // (자동) 네트워크 서브시스템 기동 확인
    GetSubsystem<UURPNetworkSubsystem>();
    GetSubsystem<UURPUISubsystem>();
    GetSubsystem<UURPLevelTransitionSubsystem>();
    GetSubsystem<UURPGameDataSubsystem>();
    GetSubsystem<UURPUserDataSubsystem>();
}


void UURPGameInstance::Shutdown()
{
    Super::Shutdown();
    // 필요 시 싱글톤 정리 로직 추가
}