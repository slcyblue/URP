// Fill out your copyright notice in the Description page of Project Settings.


#include "URPLoginWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "../Singletons/URPNetworkManager.h"
#include "../Networks/LoginRPCHandler.h"
#include "../URPGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UURPLoginWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (LoginButton)
    {
        LoginButton->OnClicked.AddDynamic(this, &UURPLoginWidget::OnLoginClicked);
    }
}

void UURPLoginWidget::OnLoginClicked()
{
    FLoginRequest Req;
    Req.ID = IDTextBox->GetText().ToString();
    Req.Password = PasswordTextBox->GetText().ToString();

    auto* GI = Cast<UURPGameInstance>(GetGameInstance());
    if (GI && GI->GetNetworkManager())
    {
        GI->GetNetworkManager()->LoginHandler->Server_LoginRequest(Req);
    }
}