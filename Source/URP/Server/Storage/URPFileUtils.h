#pragma once
#include "CoreMinimal.h"
#include "URPFileUtils.generated.h"

UCLASS()
class URP_API UURPFileUtils : public UObject
{
    GENERATED_BODY()

public:
    static bool EnsureDirectory(const FString& Directory);
};
