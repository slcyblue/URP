// Fill out your copyright notice in the Description page of Project Settings.


#include "URPLoginWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Core/Subsystems/URPNetworkSubsystem.h"
#include "Network/LoginRPCHandler.h"
#include "Core/URPGameInstance.h"
#include "Network/PlayerDataRPCHandler.h"
#include "Core/Subsystems/URPUISubsystem.h"
#include "Data/URPCommonEnums.h"
#include "Data/URPLoginData.h"
#include "Core/Subsystems/URPLevelTransitionSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include <Core/Subsystems/URPUserDataSubsystem.h>

void UURPLoginWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (LoginButton)
    {
        LoginButton->OnClicked.AddDynamic(this, &UURPLoginWidget::OnLoginClicked);

        if (auto* GI = GetGameInstance())
            if (auto* Net = GI->GetSubsystem<UURPNetworkSubsystem>())
                if (auto* Handler = Net->GetHandler<ULoginRPCHandler>())
                {
                    Handler->OnLoginResponse.RemoveAll(this);
                    Handler->OnLoginResponse.AddDynamic(this, &UURPLoginWidget::OnLoginResponseReceived);
                }
    }
}

void UURPLoginWidget::OnLoginClicked()
{
    FLoginRequest Req;
    Req.ID = IDTextBox->GetText().ToString();
    Req.Password = PasswordTextBox->GetText().ToString();

    // 네트워크 서브시스템 가져오기
    UGameInstance* GameInstance = GetGameInstance();
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("[LoginWidget] GameInstance is null."));
        return;
    }

    if (UURPNetworkSubsystem* NetworkSubsystem = GameInstance->GetSubsystem<UURPNetworkSubsystem>())
    {
        // 로그인 핸들러 가져오기
        if (auto* LoginHandler = NetworkSubsystem->GetHandler<ULoginRPCHandler>())
        {
            UE_LOG(LogTemp, Log, TEXT("[LoginWidget] Sending login request for ID: %s"), *Req.ID);
            LoginHandler->Server_LoginRequest(Req);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[LoginWidget] LoginRPCHandler not found in NetworkSubsystem."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("[LoginWidget] NetworkSubsystem not found."));
    }
}


void UURPLoginWidget::OnLoginResponseReceived(const FLoginResponse& Response)
{
    if (!Response.bSuccess)
    {
        // 실패 UI/팝업 표시
        if (auto* UI = GetGameInstance()->GetSubsystem<UURPUISubsystem>())
        {
            // UI->ShowPopup(FailPopupClass); 등
        }
        return;
    }

    // PlayerData 요청
    if (auto* Net = GetGameInstance()->GetSubsystem<UURPNetworkSubsystem>())
    {
        if (auto* PlayerHandler = Net->GetHandler<UPlayerDataRPCHandler>())
        {
            // 중복 바인딩 방지
            PlayerHandler->OnPlayerDataResponse.RemoveAll(this);
            PlayerHandler->OnPlayerDataResponse.AddDynamic(this, &UURPLoginWidget::OnPlayerDataLoaded);

            FPlayerDataRequest Req;
            Req.PlayerId = Response.PlayerId;
            Req.Action = TEXT("Load");
            PlayerHandler->Server_RequestPlayerData(Req);
        }
    }
}

void UURPLoginWidget::OnPlayerDataLoaded(const FPlayerDataResponse& Response)
{
    if (!Response.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("[LoginWidget] Failed to load PlayerData: %s, Go to Select Character"), *Response.Message);

        if (auto* UI = GetGameInstance()->GetSubsystem<UURPUISubsystem>())
        {
            UI->ShowScreen(EURPScreenType::CharacterSelect);
        }
        return;
    }

    // PlayerData를 UserDataManager에 캐싱
    if (auto* UserData = GetGameInstance()->GetSubsystem<UURPUserDataSubsystem>())
    {
        UserData->SetUserData(Response.PlayerData);
    }

    // 캐릭터 미생성 → 캐릭터 선택 화면
    if (Response.PlayerData.SelectedClass == EURPClassType::None)
    {
        if (auto* UI = GetGameInstance()->GetSubsystem<UURPUISubsystem>())
        {
            UI->ShowScreen(EURPScreenType::CharacterSelect);
        }
        return;
    }
    
    // Village Level로 이동
    if (auto* LevelTransition = GetGameInstance()->GetSubsystem<UURPLevelTransitionSubsystem>())
    {
        LevelTransition->AsyncTransitionToLevel(TEXT("Village"), EURPScreenType::VillageHUD);
    }

    UE_LOG(LogTemp, Log, TEXT("[LoginWidget] PlayerData loaded, moving to VillageHUD."));
}