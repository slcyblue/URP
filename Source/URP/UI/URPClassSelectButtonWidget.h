#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/URPCommonEnums.h"
#include "URPClassSelectButtonWidget.generated.h"

class UButton;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClassSelected, EURPClassType, ClassType);

UCLASS()
class URP_API UURPClassSelectButtonWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UButton* SelectButton;

    UPROPERTY(meta = (BindWidget))
    UImage* CharacterImage;

    // 해당 버튼이 어떤 클래스인지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Info")
    EURPClassType ClassType = EURPClassType::None;

    // 블루프린트에서 각 클래스별 Texture를 직접 지정 가능
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Class Info")
    UTexture2D* ClassImage;

    // 버튼 클릭 시 상위에 알림
    UPROPERTY(BlueprintAssignable)
    FOnClassSelected OnClassSelected;

    // 초기화용 함수
    UFUNCTION(BlueprintCallable)
    void InitializeButton(EURPClassType InType, UTexture2D* InImage);

    // 버튼이 선택되었는지 여부 (시각적 표시용)
    UFUNCTION(BlueprintCallable)
    void SetSelected(bool bSelected);

protected:
    UFUNCTION()
    void OnButtonClicked();
};
