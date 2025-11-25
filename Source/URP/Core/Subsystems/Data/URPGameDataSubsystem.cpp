
#include "URPGameDataSubsystem.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Kismet/GameplayStatics.h"
#include "Types/URPGameTypes.h"
#include "Core/Subsystems/Network/URPNetworkSubsystem.h"
#include "Network/GameDataRPCHandler.h"

void UURPGameDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(LogTemp, Log, TEXT("[GameDataManager] Subsystem Initialized"));

    Collection.InitializeDependency<UURPNetworkSubsystem>();

    LoadLocalFallback();

    InitializeGameData();
}


void UURPGameDataSubsystem::LoadLocalFallback()
{
    FString VersionFile = FPaths::ProjectSavedDir() / TEXT("GameDataVersion.txt");
    if (FPaths::FileExists(VersionFile))
    {
        FFileHelper::LoadFileToString(CachedVersion, *VersionFile);
    }
    else
    {
        CachedVersion = TEXT("0.0.0");
    }

    UE_LOG(LogTemp, Log, TEXT("[GameDataManager] Local version: %s"), *CachedVersion);
}

void UURPGameDataSubsystem::InitializeGameData()
{
    UGameInstance* GameInstance = GetGameInstance();
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("[GameDataManager] GameInstance is null."));
        return;
    }

    // 네트워크 서브시스템 가져오기
    if (UURPNetworkSubsystem* Network = GameInstance->GetSubsystem<UURPNetworkSubsystem>())
    {
        if (auto* Handler = Network->GetHandler<UGameDataRPCHandler>())
        {
            Handler->OnGameDataSyncResponse.RemoveAll(this);
            Handler->OnGameDataSyncResponse.AddDynamic(this, &UURPGameDataSubsystem::ApplyServerUpdate);

            FGameDataSyncRequest Req;
            Req.ClientVersion = CachedVersion;

            UE_LOG(LogTemp, Log, TEXT("[GameDataManager] Requesting game data sync. Local version: %s"), *CachedVersion);
            Handler->Server_RequestGameDataSync(Req);
            return;
        }
        UE_LOG(LogTemp, Warning, TEXT("[GameDataManager] GameDataRPCHandler not found."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[GameDataManager] NetworkSubsystem not found in GameInstance."));
    }
}

void UURPGameDataSubsystem::ApplyServerUpdate(const FGameDataSyncResponse& Response)
{
    // 서버에서 받은 데이터로 갱신
    CachedVersion = Response.NewVersion;
    GameTables = Response.UpdatedTables;

    UE_LOG(LogTemp, Log, TEXT("[GameDataManager] Updated to version %s with %d tables."),
        *Response.NewVersion, Response.UpdatedTables.Num());

    for (const FGameDataPacket& Packet : Response.UpdatedTables)
    {
        const FString& Name = Packet.TableName;

        if (Name == TEXT("MonsterTable"))
        {
            LoadMonsterTable(Packet);
        }
        else if (Name == TEXT("PathConfig"))
        {
            LoadPathConfig(Packet);
        }
        else if (Name == TEXT("ClassData"))
        {
            LoadClassData(Packet);
        }
        // "Skill_" 로 시작하는 모든 테이블을 SkillTable로 머지한다.
        else if (Name.StartsWith(TEXT("Skill_")))
        {
            LoadSkillData(Packet);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[GameDataSubsystem] Unknown table name: %s"), *Name);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("[GameDataManager] Updated to version %s with %d tables."), *Response.NewVersion, Response.UpdatedTables.Num());
}

void UURPGameDataSubsystem::LoadMonsterTable(const FGameDataPacket& Packet)
{
    MonsterTable = Packet.MonsterRows;
    UE_LOG(LogTemp, Log, TEXT("[GameDataSubsystem] MonsterTable loaded: %d rows"), MonsterTable.Num());
}

void UURPGameDataSubsystem::LoadPathConfig(const FGameDataPacket& Packet)
{
    PathConfig = Packet.PathConfigs;
    UE_LOG(LogTemp, Log, TEXT("[GameDataSubsystem] PathConfig loaded"));
}

void UURPGameDataSubsystem::LoadSkillData(const FGameDataPacket& Packet)
{
    // 여러 Skill_* 테이블이 올 수 있으므로 Append 방식으로 합친다.
    const int32 BeforeCount = SkillTable.Num();

    for (const FURPSkillRow& SrcRow : Packet.SkillDatas)
    {
        FURPSkillRow Row = SrcRow;

        // ProjectileClassPath → ProjectileClass 로딩
        if (!Row.ProjectileClassPath.IsEmpty())
        {
            UClass* ProjectileBP = LoadClass<AActor>(nullptr, *Row.ProjectileClassPath);
            if (!ProjectileBP)
            {
                UE_LOG(LogTemp, Error, TEXT("[GameDataSubsystem] Failed to load ProjectileClass (%s) : %s"),
                    *Row.SkillName, *Row.ProjectileClassPath);
            }
            else
            {
                Row.ProjectileClass = ProjectileBP;
            }
        }

        SkillTable.Add(MoveTemp(Row));
    }

    const int32 AddedCount = SkillTable.Num() - BeforeCount;

    UE_LOG(LogTemp, Log, TEXT("[GameDataSubsystem] SkillData added from '%s': +%d rows (total=%d)"),
        *Packet.TableName, AddedCount, SkillTable.Num());
}

void UURPGameDataSubsystem::LoadClassData(const FGameDataPacket& Packet)
{
     ClassTable = Packet.ClassData;

    for (FURPClassData& Row : ClassTable)
    {
        // 필요하다면 여기서 AnimClassPath -> AnimClass 로딩 (네가 Struct에 AnimClassPath를 넣었다는 가정)
        if (!Row.AnimClass && !Row.AnimClassPath.IsEmpty())
        {
            UClass* AnimBPClass = LoadClass<UAnimInstance>(nullptr, *Row.AnimClassPath);
            if (!AnimBPClass)
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to load AnimClass for Class %d : %s"),
                    static_cast<int32>(Row.ClassType), *Row.AnimClassPath);
            }
            else
            {
                Row.AnimClass = AnimBPClass;
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("[GameDataSubsystem] ClassData loaded: %d rows"), ClassTable.Num());
}

TOptional<FString> UURPGameDataSubsystem::GetPawnPathByClass(EURPClassType ClassType) const
{
    for (const auto& P : ClassTable)
    {
        if (P.ClassType == ClassType && !P.PawnClassPath.IsEmpty())
            return P.PawnClassPath;
    }
    return {};
}

const FURPMonsterRow* UURPGameDataSubsystem::GetMonsterRow(const FString ID)
{
    for (const auto& MT : MonsterTable)
    {
        if (MT.Name == ID) 
        {
            return &MT;
        }
    }
    return nullptr;
}

const FURPClassData* UURPGameDataSubsystem::GetClassData(EURPClassType ClassType)
{
    for (const auto& CT : ClassTable)
    {
        if (CT.ClassType == ClassType)
        {
            return &CT;
        }
    }
    return nullptr;
}
