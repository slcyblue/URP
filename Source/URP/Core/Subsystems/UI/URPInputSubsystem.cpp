#include "URPInputSubsystem.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/Player/URPPlayerCharacter.h"
#include "Characters/Player/URPPlayerController.h"
#include "Core/Subsystems/Data/URPGameDataSubsystem.h"
#include "UObject/SoftObjectPath.h"

void UURPInputSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UURPGameDataSubsystem* GameData = nullptr;

    if (auto* LP = GetLocalPlayer())
    {
        if (auto* World = LP->GetWorld())
        {
            if (auto* GI = World->GetGameInstance())
            {
                GameData = GI->GetSubsystem<UURPGameDataSubsystem>();
            }
        }
    }

    if (!GameData)
    {
        UE_LOG(LogTemp, Error, TEXT("[InputSubsystem] Cannot get GameDataSubsystem!"));
        return;
    }

    const FURPPathConfig pc = GameData->GetPathConfig();

    // PathConfig 기반 경로 로드
    InputContext = LoadIMC(pc.IMC);
    IA_LeftClick = LoadIA(pc.LeftClick);
    IA_RightClick = LoadIA(pc.RightClick);

    IA_SkillQ = LoadIA(pc.SkillQ);
    IA_SkillW = LoadIA(pc.SkillW);
    IA_SkillE = LoadIA(pc.SkillE);
    IA_SkillR = LoadIA(pc.SkillR);
}

UInputMappingContext* UURPInputSubsystem::LoadIMC(const FString& Path)
{
    if (Path.IsEmpty())
        return nullptr;

    FSoftObjectPath SoftPath(Path);
    UObject* LoadedObj = SoftPath.TryLoad();        // 동기 로드
    UInputMappingContext* IMC = Cast<UInputMappingContext>(LoadedObj);

    if (!IMC)
    {
        UE_LOG(LogTemp, Error, TEXT("[InputSubsystem] Failed to load IMC : %s"), *Path);
    }
    return IMC;
}

UInputAction* UURPInputSubsystem::LoadIA(const FString& Path)
{
    if (Path.IsEmpty())
        return nullptr;

    FSoftObjectPath SoftPath(Path);
    UObject* LoadedObj = SoftPath.TryLoad();
    UInputAction* IA = Cast<UInputAction>(LoadedObj);

    if (!IA)
    {
        UE_LOG(LogTemp, Error, TEXT("[InputSubsystem] Failed to load IA : %s"), *Path);
    }
    return IA;
}

void UURPInputSubsystem::SetupInput(APlayerController* PC)
{
    if (!PC) return;

    // IMC 등록
    if (auto* Sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        Sub->AddMappingContext(InputContext, 0);

    if (auto* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
    {
        if (IA_LeftClick)   EIC->BindAction(IA_LeftClick, ETriggerEvent::Started, this, &UURPInputSubsystem::OnLeftClick);
        EIC->BindAction(IA_RightClick, ETriggerEvent::Started, this, &UURPInputSubsystem::OnRightClickStarted);
        EIC->BindAction(IA_RightClick, ETriggerEvent::Triggered, this, &UURPInputSubsystem::OnRightClickTriggered);
        EIC->BindAction(IA_RightClick, ETriggerEvent::Completed, this, &UURPInputSubsystem::OnRightClickCompleted);

        if (IA_SkillQ) EIC->BindAction(IA_SkillQ, ETriggerEvent::Started, this, &UURPInputSubsystem::OnSkill_Q);
        if (IA_SkillW) EIC->BindAction(IA_SkillW, ETriggerEvent::Started, this, &UURPInputSubsystem::OnSkill_W);
        if (IA_SkillE) EIC->BindAction(IA_SkillE, ETriggerEvent::Started, this, &UURPInputSubsystem::OnSkill_E);
        if (IA_SkillR) EIC->BindAction(IA_SkillR, ETriggerEvent::Started, this, &UURPInputSubsystem::OnSkill_R);
    }
}


void UURPInputSubsystem::OnLeftClick(const FInputActionValue&)
{
    if (auto* PC = Cast<AURPPlayerCharacter>(GetLocalPlayer()->GetPlayerController(GetWorld())->GetPawn()))
        PC->CombatComponent->Attack();
}
void UURPInputSubsystem::OnSkill_Q(const FInputActionValue&)
{
    if (auto* PC = Cast<AURPPlayerCharacter>(GetLocalPlayer()->GetPlayerController(GetWorld())->GetPawn()))
        PC->CombatComponent->UseSkillSlot(0);
}


void UURPInputSubsystem::OnSkill_W(const FInputActionValue&)
{
    if (auto* PC = Cast<AURPPlayerCharacter>(GetLocalPlayer()->GetPlayerController(GetWorld())->GetPawn()))
        PC->CombatComponent->UseSkillSlot(1);
}


void UURPInputSubsystem::OnSkill_E(const FInputActionValue&)
{
    if (auto* PC = Cast<AURPPlayerCharacter>(GetLocalPlayer()->GetPlayerController(GetWorld())->GetPawn()))
        PC->CombatComponent->UseSkillSlot(2);
}


void UURPInputSubsystem::OnSkill_R(const FInputActionValue&)
{
    if (auto* PC = Cast<AURPPlayerCharacter>(GetLocalPlayer()->GetPlayerController(GetWorld())->GetPawn()))
        PC->CombatComponent->UseSkillSlot(3);
}


void UURPInputSubsystem::OnRightClickStarted(const FInputActionValue&)
{
    // 1회 클릭 → 현재 커서 위치로 단발 이동
    if (auto* PC = Cast<AURPPlayerController>(GetLocalPlayer()->GetPlayerController(GetWorld())))
    {
        PC->MoveToCursorOnce();
    }
}

void UURPInputSubsystem::OnRightClickTriggered(const FInputActionValue&)
{
    // 누르고 있는 동안 → 지속 이동 모드
    if (auto* PC = Cast<AURPPlayerController>(GetLocalPlayer()->GetPlayerController(GetWorld())))
    {
        PC->bIsMovingContinuous = true;
    }
}

void UURPInputSubsystem::OnRightClickCompleted(const FInputActionValue&)
{
    // 손 떼면 지속 이동 중지
    if (auto* PC = Cast<AURPPlayerController>(GetLocalPlayer()->GetPlayerController(GetWorld())))
    {
        PC->bIsMovingContinuous = false;
    }
}