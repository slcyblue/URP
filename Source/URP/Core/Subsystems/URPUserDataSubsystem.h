// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/URPGameData.h"
#include "Data/URPPlayerData.h"
#include "URPUserDataSubsystem.generated.h"
/**
 * 
 */

UCLASS()
class URP_API UURPUserDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    /** 서브시스템 초기화 */
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    /** 서버에서 받은 유저 데이터 설정 */
    void SetUserData(const FPlayerData& InData);

    /** 캐싱된 데이터 반환 */
    const FPlayerData& GetPlayerData() const { return CachedPlayerData; }

    // 추가: PlayerId 전용 캐시
    void SetCurrentPlayerId(const FString& InId) { CurrentPlayerId = InId; }
    const FString& GetCurrentPlayerId() const { return CurrentPlayerId; }
    bool HasCurrentPlayerId() const { return !CurrentPlayerId.IsEmpty(); }

    // (선택) 로그아웃/세션 종료 시 초기화용
    void ClearCurrentPlayerId() { CurrentPlayerId.Reset(); }

private:
    static UURPUserDataSubsystem* Instance;

    /** 캐싱된 유저 데이터 */
    FPlayerData CachedPlayerData;

    FString CurrentPlayerId;
};
