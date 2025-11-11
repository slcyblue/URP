#pragma once
#include "CoreMinimal.h"
#include "Data/URPGameData.h"

/**
 * 게임데이터 로드 공통 인터페이스
 * 모든 테이블 로더들은 이 인터페이스를 구현해야 함
 */
class IGameDataLoader
{
public:
    virtual ~IGameDataLoader() = default;

    /** JSON 파일을 로드하고 FGameDataPacket을 채워넣음 */
    virtual bool Load(const FString& FilePath, FGameDataPacket& OutPacket) = 0;
};