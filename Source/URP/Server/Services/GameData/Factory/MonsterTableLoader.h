#pragma once
#include "GameDataLoader.h"

class FMonsterTableLoader : public IGameDataLoader
{
public:
    virtual bool Load(const FString& FilePath, FGameDataPacket& OutPacket) override;
};
