#pragma once
#include "CoreMinimal.h"
#include "URPCommonEnums.h"
#include "URPGameData.generated.h"

USTRUCT(BlueprintType)
struct URP_API FURPMonsterRow
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Body;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 MaxHp = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Attack = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackRange = 120.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackSpeed = 1.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementSpeed = 400.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Skill = 0;
};


USTRUCT(BlueprintType)
struct FCharacterPreset
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EURPClassType Class = EURPClassType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PawnClassPath;
};

USTRUCT(BlueprintType)
struct FCharacterPresetTable
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FCharacterPreset> Presets;
};

USTRUCT(BlueprintType)
struct URP_API FGameDataPacket
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TableName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FURPMonsterRow> MonsterRows;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite)
    //TArray<FURPItemRow> ItemRows;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite)
    //TArray<FURPQuestRow> QuestRows;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCharacterPresetTable CharacterPresets;
};

USTRUCT(BlueprintType)
struct URP_API FGameDataSyncRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ClientVersion = TEXT("0.0.0");
};

USTRUCT(BlueprintType)
struct URP_API FGameDataSyncResponse
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUpToDate = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NewVersion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FGameDataPacket> UpdatedTables;
};

USTRUCT(BlueprintType)
struct URP_API FURPInitialGameData
{
    GENERATED_BODY()

    /** 현재 서버의 기획 데이터 버전 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Version;

    /** 모든 테이블의 데이터 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FGameDataPacket> Tables;

    /** 추가적으로 필요한 확장 정보 (예: 지역명, NPC 데이터 등) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> MetaInfo;

    FURPInitialGameData()
        : Version(TEXT("0.0.0"))
    {
    }
};