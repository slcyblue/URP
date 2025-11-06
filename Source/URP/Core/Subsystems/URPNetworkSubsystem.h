#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "URPNetworkSubsystem.generated.h"

class ULoginRPCHandler;
class UGameDataRPCHandler;
class UPlayerDataRPCHandler;

UCLASS()
class URP_API UURPNetworkSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;


    /** 특정 핸들러를 타입으로 접근 */
    template <typename T>
    T* GetHandler() const
    {
        UObject* const* Found = HandlerMap.Find(T::StaticClass());
        return Found ? Cast<T>(*Found) : nullptr;
    }

private:
    /** 등록된 RPC 핸들러 맵 */
    UPROPERTY()
    TMap<UClass*, UObject*> HandlerMap;

    bool ConnectToServer();

    void CheckGameDataVersion();

private:
    bool bIsListenServer = false;

#pragma region DataSync
public:
    // 주기 동기화 켜고/끄기
    void StartPeriodicSync(float IntervalSec);
    void StopPeriodicSync();

private:
    FTimerHandle SyncTimerHandle;
    void TickSync(); // 타이머 콜백
#pragma endregion
};