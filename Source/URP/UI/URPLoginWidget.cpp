// Fill out your copyright notice in the Description page of Project Settings.


#include "URPLoginWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "../Core/Subsystems/URPNetworkSubsystem.h"
#include "../Network/LoginRPCHandler.h"
#include "../Core/URPGameInstance.h"
#include "Core/Subsystems/URPUISubsystem.h"
#include "Data/URPCommonEnums.h"
#include "Data/URPLoginData.h"
#include "Kismet/GameplayStatics.h"

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

    if (auto* UI = GetGameInstance()->GetSubsystem<UURPUISubsystem>())
    {
        if (Response.bIsNewAccount)
            UI->ShowScreen(EURPScreenType::CharacterSelect);
        else
            UI->ShowScreen(EURPScreenType::VillageHUD);
    }
}