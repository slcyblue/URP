#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Types/URPCommonEnums.h"
#include "Server/Core/URPIServerService.h"
#include "URPServerDataService.generated.h"

class UURPServerServiceBase;

/**
 * 모든 서버 서비스 등록/라우팅 매니저 (싱글톤)
 */
UCLASS()
class URP_API UURPServerDataService : public UObject
{
    GENERATED_BODY()

public:
    static UURPServerDataService* Get();
    static void Initialize();

    /** 단일 진입점: 요청 라우팅 */
    bool RouteRequest(EURPServerRequestType Type, const void* Payload, void* OutResponse = nullptr);

    /** 서비스 등록 */
    void RegisterService(UClass* ServiceClass, UURPServerServiceBase* Instance);

private:
    static UURPServerDataService* Instance;

    /** 등록된 서비스 모음 (클래스 키 기준) */
    UPROPERTY()
    TMap<UClass*, UURPServerServiceBase*> Services;
};
