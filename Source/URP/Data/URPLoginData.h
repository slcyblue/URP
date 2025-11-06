#pragma once
#include "CoreMinimal.h"
#include "URPLoginData.generated.h"

USTRUCT(BlueprintType)
struct URP_API FLoginRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Password;
};

USTRUCT(BlueprintType)
struct URP_API FLoginResponse
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Message;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsNewAccount = false;
};
