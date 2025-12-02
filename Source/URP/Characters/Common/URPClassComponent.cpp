// Fill out your copyright notice in the Description page of Project Settings.


#include "URPClassComponent.h"
#include "Characters/Player/URPPlayerCharacter.h"
#include "Characters/Common/URPStatComponent.h"
#include "Core/Subsystems/Data/URPGameDataSubsystem.h"

UURPClassComponent::UURPClassComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UURPClassComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UURPClassComponent::SetClass(EURPClassType NewClass)
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

void UURPClassComponent::ApplyClassToCharacter()
{
    AURPPlayerCharacter* PC = Cast<AURPPlayerCharacter>(GetOwner());
    if (!PC || !ClassData)
        return;

    // 1) AnimBP 변경
    if (ClassData->AnimClass)
    {
        PC->GetMesh()->SetAnimInstanceClass(ClassData->AnimClass);
    }

    // 2) 기본 스탯 적용은 StatComponent를 통해 처리
    if (PC->StatComponent)
    {
        PC->StatComponent->SetBaseStats(
            ClassData->BaseMaxHp,
            ClassData->BaseAttack,
            ClassData->BaseDefense,
            ClassData->BaseAttackSpeed,
            ClassData->BaseMoveSpeed
        );
    }
}
