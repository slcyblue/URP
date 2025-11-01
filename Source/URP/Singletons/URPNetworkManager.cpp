#include "URPNetworkManager.h"
#include "../Networks/GameDataRPCHandler.h"
#include "Net/UnrealNetwork.h"
#include "../Networks/LoginRPCHandler.h"

AURPNetworkManager::AURPNetworkManager()
{
    bReplicates = true;
    SetReplicateMovement(false);
}

void AURPNetworkManager::BeginPlay()
{
    Super::BeginPlay();

    LoginHandler = NewObject<ULoginRPCHandler>(this);
    if (LoginHandler)
        LoginHandler->Init(this);
}

void AURPNetworkManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

