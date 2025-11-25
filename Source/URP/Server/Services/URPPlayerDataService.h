#pragma once
#include "CoreMinimal.h"
#include "Server/Core/URPServerServiceBase.h"
#include "Types/URPLoginTypes.h"
#include "Types/URPPlayerTypes.h"
#include "Server/Storage/URPJsonStorage.h"
#include "URPPlayerDataService.generated.h"

/** 플레이어 데이터 로드/세이브 */
UCLASS()
class URP_API UURPPlayerDataService : public UURPServerServiceBase
{
    GENERATED_BODY()

public:
    UURPPlayerDataService();

private:
    bool OnCreatePlayer(const void* Payload, void* OutResponse);
    bool OnLoadPlayer(const void* Payload, void* OutResponse);
    bool OnSavePlayer(const void* Payload, void* OutResponse);

    bool LoadPlayerData(const FString& PlayerId, FPlayerData& OutData);
    bool SavePlayerData(const FString& PlayerId, const FPlayerData& Data);
};
