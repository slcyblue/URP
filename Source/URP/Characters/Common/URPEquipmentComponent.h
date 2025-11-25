// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/URPPlayerTypes.h"
#include "URPEquipmentComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class URP_API UURPEquipmentComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UURPEquipmentComponent();

    void ApplyEquipment(const struct FURPClassData* ClassData);

protected:
    virtual void BeginPlay() override;

private:
    AActor* MainWeaponActor = nullptr;
    AActor* OffHandActor = nullptr;

    void EquipWeapon(TSubclassOf<AActor> WeaponClass, FName SocketName);
};
