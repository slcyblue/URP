#pragma once
#include "GameDataLoader.h"

/** 캐릭터 프리셋 테이블 로더 */
class FClassDataLoader : public IGameDataLoader
{
public:
    virtual bool Load(const FString& FilePath, FGameDataPacket& OutPacket) override;
};
