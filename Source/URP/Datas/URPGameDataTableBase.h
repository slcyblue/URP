// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FURPGameDataTableBase
{
    virtual ~FURPGameDataTableBase() = default;

    /** JSON ���� �ε� */
    virtual bool LoadFromJSON(const FString& FilePath) = 0;

    /** ���̺� ���� �α� ��� */
    virtual void PrintSummary() const {}
};