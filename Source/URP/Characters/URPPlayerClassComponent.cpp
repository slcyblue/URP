// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/URPPlayerClassComponent.h"
#include "Characters/URPPlayerCharacter.h"
#include <Core/Subsystems/URPGameDataSubsystem.h>

UURPPlayerClassComponent::UURPPlayerClassComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UURPPlayerClassComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UURPPlayerClassComponent::SetClass(EURPClassType NewClass)
{
    CurrentClass = NewClass;

    AURPPlayerCharacter* PC = Cast<AURPPlayerCharacter>(GetOwner());
    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("ClassComponent: Owner is NULL"));
        return;
    }

    UURPGameDataSubsystem* GameData = PC->GetGameInstance()->GetSubsystem<UURPGameDataSubsystem>();

    if (!GameData)
    {
        UE_LOG(LogTemp, Error, TEXT("ClassComponent: GameDataSubsystem not found"));
        return;
    }

    ClassData = GameData->GetClassData(NewClass);

    if (!ClassData)
    {
        UE_LOG(LogTemp, Error, TEXT("ClassComponent: ClassData not found in Subsystem"));
        return;
    }

    ApplyClassToCharacter();
}

void UURPPlayerClassComponent::ApplyClassToCharacter()
{
    AURPPlayerCharacter* PC = Cast<AURPPlayerCharacter>(GetOwner());
    if (!PC || !ClassData)
        return;

    // AnimBP 변경
    if (ClassData->AnimClass)
        PC->GetMesh()->SetAnimInstanceClass(ClassData->AnimClass);

    // PlayerCharacter 스탯 갱신
    PC->AttackPower = ClassData->BaseAttack;
    PC->MaxHp += ClassData->BaseDefense;

    UE_LOG(LogTemp, Log, TEXT("Class applied: Attack=%.1f"), ClassData->BaseAttack);
}
