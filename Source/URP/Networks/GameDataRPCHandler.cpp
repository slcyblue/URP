#include "GameDataRPCHandler.h"
#include "../Singletons/URPNetworkManager.h"
#include "../Singletons/URPGameDataManager.h"

void UGameDataRPCHandler::Init(AURPNetworkManager* InOwner)
{
    Owner = InOwner;
}

void UGameDataRPCHandler::Server_RequestAllData_Implementation()
{
    if (!Owner || !Owner->HasAuthority()) return;

    UURPGameDataManager* Mgr = UURPGameDataManager::Get(Owner->GetWorld());
    TArray<FGameDataPacket> Packets;
    Mgr->LoadAllFromServerDisk();
    Mgr->GetAllDataPackets(Packets);

    Client_ReceiveAllData(Packets);
}

void UGameDataRPCHandler::Client_ReceiveAllData_Implementation(const TArray<FGameDataPacket>& DataArray)
{
    if (UURPGameDataManager* Mgr = UURPGameDataManager::Get(GetWorld()))
    {
        for (auto& P : DataArray)
            Mgr->ApplyData(P.DataName, P.JsonString);

        Mgr->SaveLocalCache();
        UE_LOG(LogTemp, Log, TEXT("[DataRPCHandler] Received %d data entries."), DataArray.Num());
    }
}