#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultsWidget.generated.h"

class UCodeGameInstance;

UCLASS()
class OPERATIONNIGHTFALL_API UResultsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void LoadMainMenu();
	void SetWin();
	void SetLose();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* ButtonArea;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCodeMenuButton* RestartButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCodeMenuButton* MenuButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UWidgetSwitcher* ResultSwitcher;

private:

	UCodeGameInstance* GameInstance = nullptr;

	float Time;
};
