#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/URPCommonEnums.h"
#include "Types/URPPlayerTypes.h"
#include <Components/Button.h>
#include "URPCharacterSelectWidget.generated.h"

class UURPClassSelectButtonWidget;

UCLASS()
class URP_API UURPCharacterSelectWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnClassSelected(EURPClassType NewClass);

    UFUNCTION()
    void OnConfirmClass();

    UFUNCTION()
    void OnPlayerDataCreated(const FPlayerDataResponse& Response);

protected:
    UPROPERTY(meta = (BindWidget))
    UURPClassSelectButtonWidget* BarbarianButton;

    UPROPERTY(meta = (BindWidget))
    UURPClassSelectButtonWidget* KnightButton;

    UPROPERTY(meta = (BindWidget))
    UURPClassSelectButtonWidget* MageButton;

    UPROPERTY(meta = (BindWidget))
    UURPClassSelectButtonWidget* RogueButton;

    UPROPERTY(meta = (BindWidget)) 
    UButton* ConfirmButton;

    // 캐릭터 이미지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Select")
    UTexture2D* WarriorImage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Select")
    UTexture2D* KnightImage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Select")
    UTexture2D* MageImage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Select")
    UTexture2D* RogueImage;

private:
    UPROPERTY() EURPClassType SelectedClass = EURPClassType::None;

    /** 모든 클래스 버튼 리스트 (편리하게 관리) */
    TArray<UURPClassSelectButtonWidget*> ClassButtons;
};
