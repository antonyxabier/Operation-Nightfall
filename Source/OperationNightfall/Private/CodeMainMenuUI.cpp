// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeMainMenuUI.h"
#include "CodeMenuButton.h"
#include "CodeMyController.h"
#include "CodeGameInstance.h"
#include "CodeMainMenuLevelMap.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"


void UCodeMainMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button_Play)
	{
		Button_Play->OnButtonClicked.AddDynamic(this, &UCodeMainMenuUI::HandlePlayClicked);
	}

	if (Button_Quit)
	{
		Button_Quit->OnButtonClicked.AddDynamic(this, &UCodeMainMenuUI::HandleQuitClicked);
	}

	if (Button_Credits)
	{
		Button_Credits->OnButtonClicked.AddDynamic(this, &UCodeMainMenuUI::HandleCreditsClicked);
	}

	if (Button_Settings)
	{
		Button_Settings->OnButtonClicked.AddDynamic(this, &UCodeMainMenuUI::HandleSettingsClicked);
	}
}

void UCodeMainMenuUI::HandlePlayClicked()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
	UCodeGameInstance* GameInstance = Cast<UCodeGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GameInstance)
	{
		GameInstance->LoadFirstLevel(); // This will set CurrentLevelIndex to 1 and load "Demo_Scene"
	}
	else
	{
		UGameplayStatics::OpenLevel(this, FName("Demo_Scene"));
	}
}

void UCodeMainMenuUI::HandleQuitClicked()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, true);
}

void UCodeMainMenuUI::HandleCreditsClicked()
{
	if (ACodeMainMenuLevelMap* LevelScript = Cast<ACodeMainMenuLevelMap>(GetWorld()->GetLevelScriptActor()))
	{
		LevelScript->StopMainMenuMusic();
	}

	RemoveFromParent();

	if (UWorld* World = GetWorld())
	{
		APlayerController* PC = World->GetFirstPlayerController();
		if (PC && CreditsMenuClass)
		{
			UUserWidget* CreditsMenu = CreateWidget<UUserWidget>(PC, CreditsMenuClass);
			if (CreditsMenu)
			{
				CreditsMenu->AddToViewport();
			}
		}
	}
}

void UCodeMainMenuUI::HandleSettingsClicked()
{
	if (ACodeMainMenuLevelMap* LevelScript = Cast<ACodeMainMenuLevelMap>(GetWorld()->GetLevelScriptActor()))
	{
		LevelScript->StopMainMenuMusic();
	}

	RemoveFromParent();

	if (UWorld* World = GetWorld())
	{
		APlayerController* PC = World->GetFirstPlayerController();
		if (PC && SettingsMenuClass)
		{
			UUserWidget* SettingsMenu = CreateWidget<UUserWidget>(PC, SettingsMenuClass);
			if (SettingsMenu)
			{
				SettingsMenu->AddToViewport();
			}
		}
	}
}