// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeSettingsMenuUI.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CodeSettingsWidget.h"
#include "GameFramework/GameUserSettings.h"

void UCodeSettingsMenuUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ButtonSave)
		ButtonSave->OnButtonClicked.AddDynamic(this, &UCodeSettingsMenuUI::HandleSaveClicked);

	if (ButtonReset)
		ButtonReset->OnButtonClicked.AddDynamic(this, &UCodeSettingsMenuUI::HandleResetClicked);

	if (ButtonBack)
		ButtonBack->OnButtonClicked.AddDynamic(this, &UCodeSettingsMenuUI::HandleBackClicked);

	if (ButtonDontApply)
		ButtonDontApply->OnButtonClicked.AddDynamic(this, &UCodeSettingsMenuUI::HandleDontApplyClicked);

	if (ButtonSaveChanges)
		ButtonSaveChanges->OnButtonClicked.AddDynamic(this, &UCodeSettingsMenuUI::HandleSaveChangesClicked);
}

void UCodeSettingsMenuUI::NativeConstruct()
{
	
}

void UCodeSettingsMenuUI::HandleSaveClicked()
{
	ApplySettings();
	SaveSettings();
}

void UCodeSettingsMenuUI::HandleResetClicked()
{
	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->SetToDefaults();
		LoadSettings();
	}
}

void UCodeSettingsMenuUI::HandleBackClicked()
{
	ApplyChanges(bIsSettingsSaved);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC) return;

	// Show Pause or Main Menu
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		if (PauseMenuClass)
		{
			if (UUserWidget* PauseMenu = CreateWidget<UUserWidget>(PC, PauseMenuClass))
			{
				PauseMenu->AddToViewport();
			}
		}
	}
	else
	{
		if (MainMenuClass)
		{
			if (UUserWidget* MainMenu = CreateWidget<UUserWidget>(PC, MainMenuClass))
			{
				MainMenu->AddToViewport();
			}
		}
	}

	RemoveFromParent();
}

void UCodeSettingsMenuUI::HandleDontApplyClicked()
{
	// Revert temp changes
}

void UCodeSettingsMenuUI::HandleSaveChangesClicked()
{
	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->SetShadowQuality(ShadowQuality);
		Settings->SetTextureQuality(TextureQuality);

		Settings->ApplySettings(false);
		Settings->SaveSettings();

		bIsSettingsSaved = true;

		HandleBackClicked();
	}
}

void UCodeSettingsMenuUI::HandlePauseInput()
{
	RemoveFromParent();
}

void UCodeSettingsMenuUI::SetShadowQualityButton(int32 InShadowQuality)
{
	// Set internal variable
	ShadowQuality = InShadowQuality;

	// Safety check
	if (WB_Settings)
	{
		WB_Settings->LoadActiveButton(WB_Settings->ShadowQualityButtons, ShadowQuality);
	}
}

void UCodeSettingsMenuUI::ApplySettings()
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (!Settings) return;

	Settings->SetShadowQuality(ShadowQuality);
	Settings->SetTextureQuality(TextureQuality); 

	Settings->ApplySettings(false);
}

bool UCodeSettingsMenuUI::ApplyChanges(bool Condition)
{
	if (Condition)
	{
		bIsSettingsSaved = true;
		return true;
	}

	if (ButtonSaveChanges)
	{
		ButtonSaveChanges->SetVisibility(ESlateVisibility::Visible);
	}

	bIsSettingsSaved = false;
	return false;
}

void UCodeSettingsMenuUI::SaveSettings()
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (Settings)
	{
		Settings->SaveSettings();
	}
}

void UCodeSettingsMenuUI::LoadSettings()
{
	if (!WB_Settings) return;

	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (!Settings) return;

	ShadowQuality = Settings->GetShadowQuality();
	WB_Settings->LoadActiveButton(WB_Settings->ShadowQualityButtons, ShadowQuality);
	WB_Settings->LoadActiveButton(WB_Settings->TextureQualityButtons, TextureQuality);
}