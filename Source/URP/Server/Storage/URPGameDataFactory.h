#pragma once
#include "CoreMinimal.h"
#include "URPGameDataFactory.h"
#include "Types/URPGameTypes.h"
#include "GameDataLoader.h"

/**
 * 테이블명 기반으로 적절한 로더를 반환하는 팩토리 클래스
 */
class UURPGameDataFactory
{
public:
    /** 로더 등록 */
    static void RegisterDefaults();

    /** 팩토리 호출: TableName에 맞는 로더로부터 Packet 생성 */
    static bool BuildPacket(const FString& TableName, const FString& FilePath, FGameDataPacket& OutPacket);

private:
    static TMap<FString, TSharedPtr<IGameDataLoader>> Loaders;
};
