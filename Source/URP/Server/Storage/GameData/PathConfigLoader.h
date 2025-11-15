#pragma once
#include "GameDataLoader.h"

class FPathConfigLoader : public IGameDataLoader
{
public:
    virtual bool Load(const FString& FilePath, FGameDataPacket& OutPacket) override;
};
