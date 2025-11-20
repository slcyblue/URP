// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/URPPlayerData.h"
#include "URPPlayerClassComponent.generated.h"

UCLASS()
class URP_API UURPPlayerClassComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
    UURPPlayerClassComponent();

    void SetClass(EURPClassType NewClass);

    FURPClassData* GetClassData() const { return ClassData; }

protected:
    virtual void BeginPlay() override;

private:
    /** 현재 직업 */
    UPROPERTY(EditAnywhere, Category = "Class")
    EURPClassType CurrentClass = EURPClassType::None;

    /** DataTable에서 불러온 직업 데이터 */
    FURPClassData* ClassData = nullptr;

    void ApplyClassToCharacter();
};
