#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPSWidget.generated.h"

UCLASS()
class OPERATIONNIGHTFALL_API UFPSWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* FPSText;
};
