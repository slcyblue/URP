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
    /** �̱��� ���� */
    static URPGameDataManager& Get();

    void Initialize();

    /** ���̺� ��� */
    void RegisterTable(FName TableName, TSharedPtr<FURPGameDataTableBase> Table);

    /** ���̺� �������� */
    FURPGameDataTableBase* GetRaw(FName TableName) const;

    template<typename T>
    T* GetTable(FName TableName) const
    {
        return static_cast<T*>(GetRaw(TableName));
    }

    /** ��� ������ ���� */
    void Clear();

private:
    URPGameDataManager() = default;
    ~URPGameDataManager() = default;

private:
    TMap<FName, TSharedPtr<FURPGameDataTableBase>> Tables;
};
