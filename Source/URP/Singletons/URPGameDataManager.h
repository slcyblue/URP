// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Datas/URPGameDataTableBase.h"
/**
 * 
 */
class URP_API URPGameDataManager
{
public:
    /** 싱글톤 접근 */
    static URPGameDataManager& Get();

    void Initialize();

    /** 테이블 등록 */
    void RegisterTable(FName TableName, TSharedPtr<FURPGameDataTableBase> Table);

    /** 테이블 가져오기 */
    FURPGameDataTableBase* GetRaw(FName TableName) const;

    template<typename T>
    T* GetTable(FName TableName) const
    {
        return static_cast<T*>(GetRaw(TableName));
    }

    /** 모든 데이터 제거 */
    void Clear();

private:
    URPGameDataManager() = default;
    ~URPGameDataManager() = default;

private:
    TMap<FName, TSharedPtr<FURPGameDataTableBase>> Tables;
};
