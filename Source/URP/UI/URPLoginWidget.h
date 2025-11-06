// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "URPLoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class URP_API UURPLoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnLoginClicked();

    UFUNCTION()
    void OnLoginResponseReceived(const FLoginResponse& Response);

    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* IDTextBox;

    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* PasswordTextBox;

    UPROPERTY(meta = (BindWidget))
    class UButton* LoginButton;
};
