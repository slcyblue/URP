#pragma once
#include "CoreMinimal.h"
#include "Data/URPCommonEnums.h"
#include "Data/URPSkillData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "URPGameData.generated.h"

USTRUCT(BlueprintType)
struct URP_API FURPMonsterRow
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 MaxHp = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Attack = 10.f;

    // 외형(스켈레탈메시)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MeshPath;
};

USTRUCT(BlueprintType)
struct FURPPathConfig
{
    GENERATED_BODY()
    
    // Monster Class
    UPROPERTY()
    FString MonsterBPBase;

    UPROPERTY()
    FString DefaultBPName;

    UPROPERTY()
    FString MonsterMeshBase;

    UPROPERTY()
    FString DefaultMonsterAnimBP;

    UPROPERTY()
    FString DefaultMonsterBT;

    UPROPERTY()
    FString DefaultMonsterDeath;

    // Player Class
    UPROPERTY()
    FString PlayerBPBase;

    UPROPERTY()
    FString PlayerMeshBase;

    UPROPERTY()
    FString DefaultPlayerAnimBP;

    UPROPERTY()
    TMap<FString, FString> PlayerClassMap;
};


USTRUCT(BlueprintType)
struct FURPClassData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EURPClassType ClassType = EURPClassType::None;

    // 기존 값
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UAnimInstance> AnimClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PawnClassPath;

    // === 클래스 기본 스탯 ===
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 BaseMaxHp = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseAttack = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseDefense = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseMoveSpeed = 600.f;
};

USTRUCT(BlueprintType)
struct URP_API FGameDataPacket
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TableName;

    // 실제 데이터
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FURPPathConfig PathConfigs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FURPMonsterRow> MonsterRows;

    /*UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FURPItemRow> ItemRows;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FURPQuestRow> QuestRows;*/

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FURPSkillRow> SkillDatas;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FURPClassData> ClassData;
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