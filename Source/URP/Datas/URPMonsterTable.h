
#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "URPGameDataTableBase.h"
#include "URPMonsterTable.generated.h"

USTRUCT(BlueprintType)
struct FURPMonsterRow
{
public:
    GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Body;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Skill;
};

struct FURPMonsterTable : public FURPGameDataTableBase
{
    TArray<FURPMonsterRow> Rows;

    virtual bool LoadFromJSON(const FString& FilePath) override
    {
        FString FullPath = FPaths::ProjectContentDir() + FilePath;
        FString FileData;
        if (!FFileHelper::LoadFileToString(FileData, *FullPath))
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load JSON: %s"), *FullPath);
            return false;
        }

        if (!FJsonObjectConverter::JsonArrayStringToUStruct(FileData, &Rows, 0, 0))
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON: %s"), *FullPath);
            return false;
        }

        return true;
    }

    virtual void PrintSummary() const override
    {
        UE_LOG(LogTemp, Log, TEXT("FMyStatTable: %d rows"), Rows.Num());
    }
};