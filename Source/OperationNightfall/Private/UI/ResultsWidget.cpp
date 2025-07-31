#include "UI/ResultsWidget.h"
#include "CodeGameInstance.h"
#include "ONGameModeBase.h"
#include "Components/VerticalBox.h"
#include "CodeMenuButton.h"
#include "Components/WidgetSwitcher.h"

void UResultsWidget::NativeConstruct() {
	Time = 2.0f;

	if (UGameInstance* GI = GetGameInstance()) {
		GameInstance = Cast<UCodeGameInstance>(GI);
	}
	if (MenuButton) {
		MenuButton->OnButtonClicked.AddDynamic(GameInstance, &UCodeGameInstance::LoadMainMenu);
	}
	if (RestartButton && GameInstance) {
		RestartButton->OnButtonClicked.AddDynamic(GameInstance, &UCodeGameInstance::LoadCurrentLevel);
	}
}

void UResultsWidget::LoadMainMenu() {
	MenuButton->OnButtonClicked.Broadcast();
}

void UResultsWidget::SetWin() {
   
    if (ButtonArea)
    {
        ButtonArea->SetVisibility(ESlateVisibility::Visible);
        if (ResultSwitcher)
        {
            ResultSwitcher->SetActiveWidgetIndex(1);
            //Set Timer to go to the main menu
            //FTimerHandle TimerHandle;
            //GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UResultsWidget::LoadMainMenu, Time, false);
            

        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("ERROR: We need the ResultSwitch!"));
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ERROR: We need the ResultSwitch!");

        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ERROR: We need the ButtonArea!"));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "ERROR: We need the ButtonArea!");
    }
    
}


