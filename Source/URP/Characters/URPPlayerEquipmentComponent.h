// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/URPPlayerData.h"
#include "URPPlayerEquipmentComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class URP_API UURPPlayerEquipmentComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UURPPlayerEquipmentComponent();

    void ApplyEquipment(const struct FURPClassData* ClassData);

protected:
    virtual void BeginPlay() override;

private:
    AActor* MainWeaponActor = nullptr;
    AActor* OffHandActor = nullptr;

    void EquipWeapon(TSubclassOf<AActor> WeaponClass, FName SocketName);
};
