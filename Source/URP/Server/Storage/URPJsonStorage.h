#pragma once
#include "CoreMinimal.h"
#include "Server/Storage/URPDataStorage.h"
#include "URPJsonStorage.generated.h"

UCLASS()
class URP_API UURPJsonStorage : public UURPDataStorage
{
    GENERATED_BODY()

public:
    virtual bool SaveStruct(const FString& Category,
        const FString& Key,
        const void* StructData,
        const UStruct* StructType) override;

    virtual bool LoadStruct(const FString& Category,
        const FString& Key,
        void* OutStruct,
        const UStruct* StructType) override;

    virtual bool Exists(const FString& Category,
        const FString& Key) const override;

private:
    FString GetFullPath(const FString& Category, const FString& Key) const;
    bool EnsureCategoryDir(const FString& Category) const;
};
