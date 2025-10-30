// Fill out your copyright notice in the Description page of Project Settings.


#include "URPGameInstanceSubsystem.h"
#include "Singletons/URPGameDataManager.h"

UURPGameInstanceSubsystem::UURPGameInstanceSubsystem() : Super()
{

} 

void UURPGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
       //�ʿ� �̱��� �ʱ�ȭ
    URPGameDataManager::Get().Initialize();
}

void UURPGameInstanceSubsystem::Deinitialize()
{
    Super::Deinitialize();
}
