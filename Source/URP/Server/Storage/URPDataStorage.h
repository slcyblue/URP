#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "URPDataStorage.generated.h"

UCLASS(Abstract)
class URP_API UURPDataStorage : public UObject
{
    GENERATED_BODY()

public:
    // 모든 시그니처는 const/포인터 타입까지 정확히 일치해야 함
    virtual bool SaveStruct(const FString& Category,
        const FString& Key,
        const void* StructData,
        const UStruct* StructType) PURE_VIRTUAL(UURPDataStorage::SaveStruct, return false;);

    virtual bool LoadStruct(const FString& Category,
        const FString& Key,
        void* OutStruct,
        const UStruct* StructType) PURE_VIRTUAL(UURPDataStorage::LoadStruct, return false;);

    virtual bool Exists(const FString& Category,
        const FString& Key) const PURE_VIRTUAL(UURPDataStorage::Exists, return false;);
};
