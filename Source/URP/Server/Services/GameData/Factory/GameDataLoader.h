#pragma once
#include "CoreMinimal.h"
#include "Data/URPGameData.h"

class IGameDataLoader
{
public:
    virtual ~IGameDataLoader() = default;
    virtual bool Load(const FString& FilePath, FGameDataPacket& OutPacket) = 0;
};
