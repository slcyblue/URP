#pragma once
#include "CoreMinimal.h"
#include "GameDataLoader.h"

class UGameDataFactory
{
public:
    static void RegisterDefaults();
    static bool BuildPacket(const FString& TableName, const FString& FilePath, FGameDataPacket& OutPacket);

private:
    static TMap<FString, TSharedPtr<IGameDataLoader>> Loaders;
};
