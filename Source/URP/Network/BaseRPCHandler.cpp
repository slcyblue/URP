#include "BaseRPCHandler.h"
#include "URP/Core//Subsystems/URPNetworkSubsystem.h"

void UBaseRPCHandler::Initialize(UURPNetworkSubsystem* InNetwork)
{
    NetworkSubsystem = InNetwork;
}

UGameInstance* UBaseRPCHandler::GetGameInstance() const
{
    // 1️⃣ Handler가 World 기반 객체라면
    if (const UWorld* World = GetWorld())
    {
        return World->GetGameInstance();
    }

    // 2️⃣ Outer를 통해 접근 (NetworkSubsystem 등)
    if (const UObject* OuterObj = GetOuter())
    {
        if (const UWorld* OuterWorld = OuterObj->GetWorld())
        {
            return OuterWorld->GetGameInstance();
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("[BaseHandlerRpc] Failed to resolve GameInstance."));
    return nullptr;
}