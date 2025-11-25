#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "URPIServerService.h"
#include "Types/URPCommonEnums.h"
#include "Server/Storage//URPJsonStorage.h"
#include "URPServerServiceBase.generated.h"

/** 요청 처리 델리게이트: Payload + OutResponse 모두 전달 */
DECLARE_DELEGATE_RetVal_TwoParams(bool, FServerRequestDelegate, const void* /*Payload*/, void* /*OutResponse*/);

/**
 * 공통 베이스: 요청타입→핸들러 자동 등록/실행
 */
UCLASS(Abstract)
class URP_API UURPServerServiceBase : public UObject, public IURPIServerService
{
    GENERATED_BODY()


public:
    virtual void Initialize()
    {
        if (!Storage && !HasAnyFlags(RF_ClassDefaultObject))
        {
            Storage = NewObject<UURPJsonStorage>(this);
            UE_LOG(LogTemp, Log, TEXT("[ServerServiceBase] Storage initialized for %s"), *GetName());
        }
    }

    /** 핸들러 등록 (Payload + OutResponse 기반) */
    template<typename T>
    void RegisterHandler(EURPServerRequestType Type, bool (T::* Func)(const void*, void*))
    {
        Handlers.Add(Type, FServerRequestDelegate::CreateUObject(static_cast<T*>(this), Func));
    }

    // 오버로드: Response 없는 단순 버전
    template<typename T>
    void RegisterHandler(EURPServerRequestType Type, bool (T::* Func)(const void*))
    {
        Handlers.Add(Type, FServerRequestDelegate::CreateLambda(
            [this, Func](const void* Payload, void*)
            {
                return (static_cast<T*>(this)->*Func)(Payload);
            }));
    }

    /** 요청 실행 (자동으로 등록된 핸들러 호출) */
    virtual bool HandleServerRequest(EURPServerRequestType Type, const void* Payload, void* OutResponse) override
    {
        if (const FServerRequestDelegate* Handler = Handlers.Find(Type))
        {
            return Handler->Execute(Payload, OutResponse);
        }
        return false;
    }

protected:
    TMap<EURPServerRequestType, FServerRequestDelegate> Handlers;

    UPROPERTY()
    UURPJsonStorage* Storage;
};
