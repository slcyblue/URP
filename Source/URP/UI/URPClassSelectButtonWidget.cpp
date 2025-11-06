#include "URPClassSelectButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"


void UURPClassSelectButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SelectButton)
        SelectButton->OnClicked.AddDynamic(this, &UURPClassSelectButtonWidget::OnButtonClicked);

    if (CharacterImage && ClassImage)
        CharacterImage->SetBrushFromTexture(ClassImage, true);
}

void UURPClassSelectButtonWidget::InitializeButton(EURPClassType InType, UTexture2D* InImage)
{
    ClassType = InType;
    ClassImage = InImage;

    if (CharacterImage && InImage)
        CharacterImage->SetBrushFromTexture(InImage, true);
}

void UURPClassSelectButtonWidget::OnButtonClicked()
{
    OnClassSelected.Broadcast(ClassType);
}

void UURPClassSelectButtonWidget::SetSelected(bool bSelected)
{
    // 여기서 선택 시 색상 변경 등 처리 (예: Tint 적용)
    if (CharacterImage)
    {
        const FLinearColor Color = bSelected ? FLinearColor(1.f, 1.f, 1.f, 1.f)
            : FLinearColor(0.5f, 0.5f, 0.5f, 1.f);
        CharacterImage->SetColorAndOpacity(Color);
    }
}