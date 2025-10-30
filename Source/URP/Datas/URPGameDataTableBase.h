// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FURPGameDataTableBase
{
    virtual ~FURPGameDataTableBase() = default;

    /** JSON 파일 로드 */
    virtual bool LoadFromJSON(const FString& FilePath) = 0;

    /** 테이블 정보 로그 출력 */
    virtual void PrintSummary() const {}
};