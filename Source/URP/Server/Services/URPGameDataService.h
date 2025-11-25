#pragma once
#include "CoreMinimal.h"
#include "Server/Core/URPServerServiceBase.h"
#include "Types/URPGameTypes.h"
#include "Server/Storage/URPJsonStorage.h"
#include "URPGameDataService.generated.h"

/** 기획 데이터 동기화 */
UCLASS()
class URP_API UURPGameDataService : public UURPServerServiceBase
{
    GENERATED_BODY()

public:
    UURPGameDataService();

private:
    bool OnSyncGameData(const void* Payload, void* OutResponse);

    // TODO: 실제 서버 GameData 버전 비교/패킷 구성
    bool BuildSyncResponse(const struct FGameDataSyncRequest& Req, struct FGameDataSyncResponse& Out);
};
