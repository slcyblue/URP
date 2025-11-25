#pragma once
#include "CoreMinimal.h"
#include "URPCommonEnums.generated.h"


UENUM(BlueprintType)
enum class EURPServerRequestType : uint8
{
    None        UMETA(DisplayName = "None"),
    Login       UMETA(DisplayName = "Account Login"),
    CreatePlayer  UMETA(DisplayName = "Create Account"),
    VerifyLogin  UMETA(DisplayName = "Verify Account"),
    LoadPlayer  UMETA(DisplayName = "Load Player Data"),
    SavePlayer  UMETA(DisplayName = "Save Player Data"),
    SyncGameData UMETA(DisplayName = "Sync Game Data")
};

UENUM(BlueprintType)
enum class EURPScreenType : uint8
{
    None,
    Login,
    CharacterSelect,
    VillageHUD,
    Loading,
    Popup
};

UENUM(BlueprintType)
enum class EURPClassType : uint8
{
    None UMETA(DisplayName = "None"),
    Barbarian UMETA(DisplayName = "Barbarian"),
    Knight UMETA(DisplayName = "Knight"),
    Mage UMETA(DisplayName = "Mage"),
    Rogue UMETA(DisplayName = "Rogue")
};

UENUM(BlueprintType)
enum class EURPMonsterType : uint8
{
    None UMETA(DisplayName = "None"),
    SkeletonMinion UMETA(DisplayName = "SK_Minion"),
    SkeletonKnight UMETA(DisplayName = "SK_Knight"),
    SkeletonMage UMETA(DisplayName = "SK_Mage"),
    SkeletonRogue UMETA(DisplayName = "SK_Rogue")
};

UENUM(BlueprintType)
enum class EAIState : uint8
{
    Idle,
    Patrol,
    Chase,
    Attack,
    Return,
    Dead
};

UENUM(BlueprintType)
enum class EURPSkillType : uint8
{
    None        UMETA(DisplayName = "None"),
    SingleHit   UMETA(DisplayName = "SingleHit"),
    AOE         UMETA(DisplayName = "AOE"),
    Projectile  UMETA(DisplayName = "Projectile"),
    Dash        UMETA(DisplayName = "Dash"),
    DOT        UMETA(DisplayName = "DOT"),
    Buff        UMETA(DisplayName = "Buff"),
    Debuff        UMETA(DisplayName = "Debuff"),
};

UENUM(BlueprintType)
enum class EURPBuffType : uint8
{
    None,
    AttackUp,
    DefenseUp,
    MoveSpeedUp,
    SkillHaste,
    Shield
};

UENUM(BlueprintType)
enum class EURPDebuffType : uint8
{
    None,
    AttackDown,
    DefenseDown,
    MoveSpeedDown,
    Silence,
    Stun
};