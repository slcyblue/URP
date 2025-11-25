// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Types/URPGameTypes.h"
#include "URPClassComponent.generated.h"

UCLASS()
class URP_API UURPClassComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
    UURPClassComponent();

    void SetClass(EURPClassType NewClass);

    const FURPClassData* GetClassData() { return ClassData; }

protected:
    virtual void BeginPlay() override;

private:
    /** 현재 직업 */
    UPROPERTY(EditAnywhere, Category = "Class")
    EURPClassType CurrentClass = EURPClassType::None;

    /** DataTable에서 불러온 직업 데이터 */
    const FURPClassData* ClassData = nullptr;

    void ApplyClassToCharacter();
};
