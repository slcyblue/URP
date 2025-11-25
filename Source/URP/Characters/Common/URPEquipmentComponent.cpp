#include "URPEquipmentComponent.h"
#include "Characters/Player/URPPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

UURPEquipmentComponent::UURPEquipmentComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UURPEquipmentComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UURPEquipmentComponent::ApplyEquipment(const FURPClassData* ClassData)
{
    if (!ClassData)
        return;

    AURPPlayerCharacter* PC = Cast<AURPPlayerCharacter>(GetOwner());
    if (!PC)
        return;

    // 이전 무기 삭제
    if (MainWeaponActor)
        MainWeaponActor->Destroy();
    if (OffHandActor)
        OffHandActor->Destroy();

    // 메인 무기
    //if (ClassData->MainWeapon)
    //{
    //    EquipWeapon(ClassData->MainWeapon, "hand_r");
    //}

    //// 보조 무기(방패/보조무기)
    //if (ClassData->bUseShield && ClassData->OffHandItem)
    //{
    //    EquipWeapon(ClassData->OffHandItem, "hand_l");
    //}
}

void UURPEquipmentComponent::EquipWeapon(TSubclassOf<AActor> WeaponClass, FName SocketName)
{
    AURPPlayerCharacter* PC = Cast<AURPPlayerCharacter>(GetOwner());
    if (!PC || !WeaponClass)
        return;

    FActorSpawnParameters Params;
    Params.Owner = PC;

    AActor* Weapon = GetWorld()->SpawnActor<AActor>(WeaponClass, Params);
    Weapon->AttachToComponent(PC->GetMesh(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        SocketName);

    if (SocketName == "hand_r")
        MainWeaponActor = Weapon;
    else
        OffHandActor = Weapon;
}