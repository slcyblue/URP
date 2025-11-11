#pragma once
#include "GameDataLoader.h"

/** 몬스터 테이블 로더 */
class FMonsterTableLoader : public IGameDataLoader
{
public:
    virtual bool Load(const FString& FilePath, FGameDataPacket& OutPacket) override;
};
