#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseRPCHandler.generated.h"

class UURPNetworkSubsystem;

UCLASS(Abstract)
class URP_API UBaseRPCHandler : public UObject
{
    GENERATED_BODY()

public:
    virtual void Initialize(UURPNetworkSubsystem* InSubsystem);

protected:
    UPROPERTY()
    UURPNetworkSubsystem* NetworkSubsystem;
};
