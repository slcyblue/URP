#include "Server/URPServerDataService.h"
#include "Server/Core/URPServiceRegistry.h"
#include "Server/Services/URPAccountService.h"
#include "Server/Services/URPPlayerDataService.h"
#include "Server/Services/URPGameDataService.h"

UURPServerDataService* UURPServerDataService::Instance = nullptr;

UURPServerDataService* UURPServerDataService::Get()
{
    if (!Instance)
    {
        Instance = NewObject<UURPServerDataService>();
        Instance->AddToRoot();
        Initialize();
    }
    return Instance;
}

void UURPServerDataService::Initialize()
{
    UURPServerDataService* Owner = Get();

    // 자동 등록
    REGISTER_SERVER_SERVICE(UURPAccountService, Owner);
    REGISTER_SERVER_SERVICE(UURPPlayerDataService, Owner);
    REGISTER_SERVER_SERVICE(UURPGameDataService, Owner);

    UE_LOG(LogTemp, Log, TEXT("[ServerDataService] All services initialized."));
}

void UURPServerDataService::RegisterService(UClass* ServiceClass, UURPServerServiceBase* ServiceInstance)
{
    if (!ServiceClass || !ServiceInstance) return;
    Services.Add(ServiceClass, ServiceInstance);
}

bool UURPServerDataService::RouteRequest(EURPServerRequestType Type, const void* Payload, void* OutResponse)
{
    for (auto& Pair : Services)
    {
        if (Pair.Value && Pair.Value->HandleServerRequest(Type, Payload, OutResponse))
        {
            return true;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("[ServerDataService] Unhandled request type: %d"), (int32)Type);
    return false;
}
