#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "InputActionValue.h"
#include "URPInputSubsystem.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class URP_API UURPInputSubsystem : public ULocalPlayerSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    void SetupInput(class APlayerController* PC);

private:
    UInputMappingContext* LoadIMC(const FString& Path);
    UInputAction* LoadIA(const FString& Path);

    UInputMappingContext* InputContext = nullptr;
    UInputAction* IA_LeftClick = nullptr;
    UInputAction* IA_RightClick = nullptr;
    UInputAction* IA_SkillQ = nullptr;
    UInputAction* IA_SkillW = nullptr;
    UInputAction* IA_SkillE = nullptr;
    UInputAction* IA_SkillR = nullptr;

private:
    void OnLeftClick(const FInputActionValue& Value);
    void OnRightClickStarted(const FInputActionValue& Value);
    void OnRightClickTriggered(const FInputActionValue& Value);
    void OnRightClickCompleted(const FInputActionValue& Value);

    void OnSkill_Q(const FInputActionValue& Value);
    void OnSkill_W(const FInputActionValue& Value);
    void OnSkill_E(const FInputActionValue& Value);
    void OnSkill_R(const FInputActionValue& Value);
};
