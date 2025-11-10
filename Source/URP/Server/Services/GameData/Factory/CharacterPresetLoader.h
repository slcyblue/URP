#pragma once
#include "GameDataLoader.h"

class FCharacterPresetLoader : public IGameDataLoader
{
public:
    virtual bool Load(const FString& FilePath, FGameDataPacket& OutPacket) override;
};
