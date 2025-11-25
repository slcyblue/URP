#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Types/URPCommonEnums.h"
#include "URPIServerService.generated.h"

UINTERFACE(MinimalAPI)
class UURPIServerService : public UInterface
{
    GENERATED_BODY()
};

class URP_API IURPIServerService
{
    GENERATED_BODY()

public:
    /** 서비스가 요청을 처리할 수 있으면 true 반환 */
    virtual bool HandleServerRequest(EURPServerRequestType Type, const void* Payload, void* OutResponse) = 0;
};
