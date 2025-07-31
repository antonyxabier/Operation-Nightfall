#include "UI/FPSWidget.h"
#include "Components/TextBlock.h"

void UFPSWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	float FPS = 1 / InDeltaTime;
	FString FPSString = FString::Printf(TEXT("FPS: %.0f"), FPS);
	FPSText->SetText(FText::FromString(FPSString));
}