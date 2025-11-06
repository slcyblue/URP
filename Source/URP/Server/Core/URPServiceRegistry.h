#pragma once
#include "CoreMinimal.h"

/** 서비스 자동 등록 헬퍼 매크로 */
#define REGISTER_SERVER_SERVICE(ServiceClass, Owner)                                              \
    do {                                                                                          \
        if (!(Owner)) {                                                                           \
            UE_LOG(LogTemp, Error, TEXT("[ServerServiceRegistry] Owner is null! (%s)"), TEXT(#ServiceClass)); \
            break;                                                                                \
        }                                                                                         \
                                                                                                  \
        UURPServerServiceBase* ServiceInstance = NewObject<ServiceClass>(Owner);                  \
        if (!ServiceInstance || ServiceInstance->HasAnyFlags(RF_ClassDefaultObject)) {            \
            UE_LOG(LogTemp, Error, TEXT("[ServerServiceRegistry] Failed to create %s."), TEXT(#ServiceClass)); \
            break;                                                                                \
        }                                                                                         \
                                                                                                  \
        /* Base Initialize() 호출로 Storage 생성 */                                             \
        ServiceInstance->Initialize();                                                            \
                                                                                                  \
        /* 서비스 등록 */                                                                     \
        (Owner)->RegisterService(ServiceClass::StaticClass(), ServiceInstance);                   \
                                                                                                  \
        UE_LOG(LogTemp, Log, TEXT("[ServerDataService] Registered: %s"),                          \
            *ServiceClass::StaticClass()->GetName());                                             \
    } while(0)
