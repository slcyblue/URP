
#include "URPGameDataSubsystem.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Kismet/GameplayStatics.h"
#include "Data/URPGameData.h"
#include "Core/Subsystems/URPNetworkSubsystem.h"
#include "Network/GameDataRPCHandler.h"
#include "Characters/Skill/URPSkillBase.h"

void UURPGameDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(LogTemp, Log, TEXT("[GameDataManager] Subsystem Initialized"));

    Collection.InitializeDependency<UURPNetworkSubsystem>();

    // 테이블 이름 -> 로더 함수 매핑
    TableLoaders.Add(TEXT("MonsterTable"),
        [this](const FGameDataPacket& Packet)
        {
            LoadMonsterTable(Packet);
        });

    TableLoaders.Add(TEXT("PathConfig"),
        [this](const FGameDataPacket& Packet)
        {
            LoadPathConfig(Packet);
        });

    TableLoaders.Add(TEXT("SkillData"),
        [this](const FGameDataPacket& Packet)
        {
            LoadSkillData(Packet);
        });

    TableLoaders.Add(TEXT("ClassData"),
        [this](const FGameDataPacket& Packet)
        {
            LoadClassData(Packet);
        });

    UE_LOG(LogTemp, Log, TEXT("[GameDataSubsystem] Initialize: TableLoaders registered."));

    LoadLocalFallback();

    InitializeGameData();
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

void UURPGameDataSubsystem::ApplyServerUpdate(const FGameDataSyncResponse& Response)
{
    // 서버에서 받은 데이터로 갱신
    CachedVersion = Response.NewVersion;
    GameTables = Response.UpdatedTables;

    UE_LOG(LogTemp, Log, TEXT("[GameDataManager] Updated to version %s with %d tables."),
        *Response.NewVersion, Response.UpdatedTables.Num());

    for (const FGameDataPacket& Packet : Response.UpdatedTables)
    {
        if (TFunction<void(const FGameDataPacket&)>* Loader = TableLoaders.Find(Packet.TableName))
        {
            // 등록된 로더 호출
            (*Loader)(Packet);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[GameDataSubsystem] Unknown table name: %s"), *Packet.TableName);
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
    SkillTable = Packet.SkillDatas;

    // 여기서 SkillClassPath -> SkillClass 로딩 + 로그
    for (FURPSkillRow& Row : SkillTable)
    {
        if (!Row.ProjectileClassPath.IsEmpty())
        {
            UClass* ProjectileBP = LoadClass<AActor>(nullptr, *Row.ProjectileClassPath);
            if (!ProjectileBP)
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to load ProjectileClass (%s) : %s"),
                    *Row.SkillName,
                    *Row.ProjectileClassPath);
            }
            else
            {
                Row.ProjectileClass = ProjectileBP;
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("[GameDataSubsystem] SkillData loaded: %d rows"), SkillTable.Num());
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
