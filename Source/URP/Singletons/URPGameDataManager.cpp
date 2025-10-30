// Fill out your copyright notice in the Description page of Project Settings.


#include "URPGameDataManager.h"
#include "../Datas/URPMonsterTable.h"

URPGameDataManager& URPGameDataManager::Get()
{
    static URPGameDataManager Instance;
    return Instance;
}

void URPGameDataManager::Initialize()
{
    UE_LOG(LogTemp, Log, TEXT("GameDataManager initialized"));

    // MyStat JSON 로드
    //auto MonsterTable = MakeShared<FURPMonsterTable>();
    /*if (MonsterTable->LoadFromJSON(TEXT("Data/MyStat.json")))
    {
        RegisterTable(TEXT("MyStat"), MonsterTable);
    }*/

    // Item JSON 로드
    /*auto ItemTable = MakeShared<FItemTable>();
    if (ItemTable->LoadFromJSON(TEXT("Data/Item.json")))
    {
        RegisterTable(TEXT("Item"), ItemTable);
    }*/

    // 로드된 테이블 로그 출력
    for (auto& Pair : Tables)
    {
        Pair.Value->PrintSummary();
    }
}

void URPGameDataManager::RegisterTable(FName TableName, TSharedPtr<FURPGameDataTableBase> Table)
{
    Tables.Add(TableName, Table);
}

FURPGameDataTableBase* URPGameDataManager::GetRaw(FName TableName) const
{
    if (const TSharedPtr<FURPGameDataTableBase>* Found = Tables.Find(TableName))
    {
        return Found->Get();
    }
    return nullptr;
}

void URPGameDataManager::Clear()
{
    Tables.Empty();
    UE_LOG(LogTemp, Log, TEXT("GameDataManager cleared"));
}