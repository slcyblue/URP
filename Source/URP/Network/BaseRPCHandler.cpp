#include "BaseRPCHandler.h"
#include "URP/Core//Subsystems/URPNetworkSubsystem.h"

void UBaseRPCHandler::Initialize(UURPNetworkSubsystem* InNetwork)
{
    NetworkSubsystem = InNetwork;
}
