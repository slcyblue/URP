#pragma once
#include "CoreMinimal.h"
#include "URPCommonEnums.h"
#include "URPSkillTypes.h"
#include "URPPlayerTypes.generated.h"

USTRUCT(BlueprintType)
struct URP_API FPlayerData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Level = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Exp = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Gold = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> InventoryItems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FSkillEntry> SkillLevels;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EURPClassType SelectedClass = EURPClassType::None;
};

USTRUCT(BlueprintType)
struct URP_API FPlayerDataRequest
{
    GENERATED_BODY()

    /** 요청 타입 (Load / Save 등) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerId;

    /** 세이브 시에는 이 값이 채워짐 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPlayerData PlayerData;

    /** 요청 구분자 (예: "Load" / "Save") */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Action;

    FPlayerDataRequest()
        : PlayerId(TEXT("")), Action(TEXT("Load"))
    {
    }
};

USTRUCT(BlueprintType)
struct URP_API FPlayerDataResponse
{
    GENERATED_BODY()

    /** 성공 여부 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess = false;

    /** 서버에서 내려주는 플레이어 데이터 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPlayerData PlayerData;

    /** 서버 응답 메시지 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Message;

    /** 요청 구분자 (예: "Load" / "Save") */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Action;

    FPlayerDataResponse()
        : bSuccess(false)
    {
    }
};
