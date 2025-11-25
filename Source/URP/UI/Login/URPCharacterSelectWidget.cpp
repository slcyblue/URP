#include "URPCharacterSelectWidget.h"
#include "URPClassSelectButtonWidget.h"
#include "Types/URPPlayerTypes.h"
#include "Core/Subsystems/Data/URPUserDataSubsystem.h"
#include "Core/Subsystems/Network/URPNetworkSubsystem.h"
#include "Core/Subsystems/UI/URPLevelTransitionSubsystem.h"
#include "Network/PlayerDataRPCHandler.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UURPCharacterSelectWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // RPC 델리게이트 연결
    if (auto* Net = GetGameInstance()->GetSubsystem<UURPNetworkSubsystem>())
        if (auto* PlayerHandler = Net->GetHandler<UPlayerDataRPCHandler>())
        {
            PlayerHandler->OnPlayerDataResponse.AddDynamic(this, &UURPCharacterSelectWidget::OnPlayerDataCreated);
        }

    // 각 버튼 초기화
    if (BarbarianButton)
    {
        BarbarianButton->InitializeButton(EURPClassType::Barbarian, WarriorImage);
        BarbarianButton->OnClassSelected.AddDynamic(this, &UURPCharacterSelectWidget::OnClassSelected);
    }

    if (KnightButton)
    {
        KnightButton->InitializeButton(EURPClassType::Knight, KnightImage);
        KnightButton->OnClassSelected.AddDynamic(this, &UURPCharacterSelectWidget::OnClassSelected);
    }

    if (MageButton)
    {
        MageButton->InitializeButton(EURPClassType::Mage, MageImage);
        MageButton->OnClassSelected.AddDynamic(this, &UURPCharacterSelectWidget::OnClassSelected);
    }

    if (RogueButton)
    {
        RogueButton->InitializeButton(EURPClassType::Rogue, RogueImage);
        RogueButton->OnClassSelected.AddDynamic(this, &UURPCharacterSelectWidget::OnClassSelected);
    }

    if (ConfirmButton)
        ConfirmButton->OnClicked.AddDynamic(this, &UURPCharacterSelectWidget::OnConfirmClass);

    // 관리 배열
    ClassButtons = { BarbarianButton, KnightButton, MageButton, RogueButton };
}

void UURPCharacterSelectWidget::OnClassSelected(EURPClassType NewClass)
{
    SelectedClass = NewClass;
    UE_LOG(LogTemp, Log, TEXT("[CharacterSelect] Selected Class: %d"), (uint8)SelectedClass);

    for (auto* Btn : ClassButtons)
    {
        if (Btn)
            Btn->SetSelected(Btn->ClassType == SelectedClass);
    }
}

void UURPCharacterSelectWidget::OnConfirmClass()
{
    if (SelectedClass == EURPClassType::None)
    {
        UE_LOG(LogTemp, Warning, TEXT("No class selected!"));
        return;
    }

    FString LoginId;
    if (auto* UserDataMgr = GetGameInstance()->GetSubsystem<UURPUserDataSubsystem>())
    {
        LoginId = UserDataMgr->GetCurrentPlayerId();

        if (LoginId.IsEmpty())
        {
            LoginId = UserDataMgr->GetPlayerData().PlayerId;
        }
    }
       
    if (LoginId.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("[CharacterSelect] Missing LoginId."));
        return;
    }

    if (auto* Net = GetGameInstance()->GetSubsystem<UURPNetworkSubsystem>())
        if (auto* PlayerHandler = Net->GetHandler<UPlayerDataRPCHandler>())
        {
            UE_LOG(LogTemp, Log, TEXT("[CharacterSelect] Init PlayerData for %s (Class=%d)"), *LoginId, (uint8)SelectedClass);
            PlayerHandler->Server_CreatePlayer(LoginId, SelectedClass);   // 신규 생성 전용 RPC
        }
}


void UURPCharacterSelectWidget::OnPlayerDataCreated(const FPlayerDataResponse& Response)
{
    if (!Response.bSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("[CharacterSelect] PlayerData creation failed: %s"), *Response.Message);
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("[CharacterSelect] PlayerData created successfully for %s"), *Response.PlayerData.PlayerId);

    if (auto* LevelSys = GetGameInstance()->GetSubsystem<UURPLevelTransitionSubsystem>())
    {
        LevelSys->AsyncTransitionToLevel(TEXT("Village"), EURPScreenType::VillageHUD);
    }
}