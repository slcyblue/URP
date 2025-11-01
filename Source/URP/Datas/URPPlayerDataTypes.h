// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "URPPlayerDataTypes.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerCharacterData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Level;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Exp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Class;
};

USTRUCT(BlueprintType)
struct FPlayerInventoryData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> OwnedItems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> EquippedItems;
};


USTRUCT(BlueprintType)
struct FSkillEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SkillId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Level;
};


USTRUCT(BlueprintType)
struct FPlayerSkillData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> SkillIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FSkillEntry> SkillLevels;
};
