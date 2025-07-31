// Fill out your copyright notice in the Description page of Project Settings.

#include "CodeSettingsWidget.h"
#include "CodeSettingsMenuUI.h"
#include "GameFramework/GameUserSettings.h"
#include "Components/Border.h"
#include "CodeMenuButton.h"

void UCodeSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BorderColor)
	{
		// Determine if game is paused
		const UWorld* World = GetWorld();
		const bool bIsPaused = World && World->IsPaused();

		FLinearColor SelectedColor = bIsPaused
			? FLinearColor(0.f, 0.f, 0.f, 0.f) // Transparent
			: FLinearColor::White;

		BorderColor->SetBrushColor(SelectedColor);
	}

	// Bind button click handlers
	if (Graphics)
	{
		Graphics->OnButtonClicked.AddDynamic(this, &UCodeSettingsWidget::HandleGraphicsClicked);
	}
	if (Audio)
	{
		Audio->OnButtonClicked.AddDynamic(this, &UCodeSettingsWidget::HandleAudioClicked);
	}
	if (Controls)
	{
		Controls->OnButtonClicked.AddDynamic(this, &UCodeSettingsWidget::HandleControlsClicked);
	}

#if WITH_EDITOR
	if (IsDesignTime())
	{
		return;
	}
#endif

	// Build Shadow quality button array
	ShadowQualityButtons = {
		ShadowQuality00,
		ShadowQuality01,
		ShadowQuality02,
		ShadowQuality03
	};

	// Build Texture quality button array
	TextureQualityButtons = {
		TextureQuality00,
		TextureQuality01,
		TextureQuality02,
		TextureQuality03
	};

	OnShadowQualityUpdated();
	OnTextureQualityUpdated();
}

void UCodeSettingsWidget::HandleGraphicsClicked()
{
	if (WidgetSwitcher_Settings)
	{
		WidgetSwitcher_Settings->SetActiveWidgetIndex(0);
	}
}

void UCodeSettingsWidget::HandleControlsClicked()
{
	if (WidgetSwitcher_Settings)
	{
		WidgetSwitcher_Settings->SetActiveWidgetIndex(1);
	}
}

void UCodeSettingsWidget::HandleAudioClicked()
{
	if (WidgetSwitcher_Settings)
	{
		WidgetSwitcher_Settings->SetActiveWidgetIndex(2);
	}
}

void UCodeSettingsWidget::CheckShadowQuality()
{
	if (!SettingsMenuUI) return;

	UGameUserSettings* GameSettings = GEngine->GetGameUserSettings();
	if (!GameSettings) return;

	int32 CurrentShadowQuality = GameSettings->GetShadowQuality();

	// Compare to the UI's stored value
	if (CurrentShadowQuality == SettingsMenuUI->ShadowQuality)
	{
		SettingsMenuUI->bIsSettingsSaved = true;
	}
	else
	{
		SettingsMenuUI->bIsSettingsSaved = false;
	}
}

UCodeMenuButton* UCodeSettingsWidget::GetActiveButton(int32 Index, TArray<TObjectPtr<UCodeMenuButton>>& TargetArray)
{
	UCodeMenuButton* Result = nullptr;

	for (int32 i = 0; i < TargetArray.Num(); ++i)
	{
		if (TargetArray[i])
		{
			TargetArray[i]->ChangeButtonStatus(false);
		}
	}

	if (TargetArray.IsValidIndex(Index) && TargetArray[Index])
	{
		TargetArray[Index]->ChangeButtonStatus(true);
		Result = TargetArray[Index];
	}

	return Result;
}

void UCodeSettingsWidget::GetAndApplySettings()
{
	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->ApplySettings(false);
	}
}

void UCodeSettingsWidget::OnShadowQualityUpdated()
{
	CheckShadowQuality();

	for (int32 i = 0; i < ShadowQualityButtons.Num(); ++i)
	{
		if (ShadowQualityButtons[i])
		{
			ShadowQualityButtons[i]->OnButtonClickedWithIndex.Clear();
			ShadowQualityButtons[i]->OnButtonClickedWithIndex.AddDynamic(this, &UCodeSettingsWidget::OnShadowQualityChanged);
		}
	}
}

void UCodeSettingsWidget::SetDefaultShadowQuality()
{
	const int32 DefaultIndex = 3;

	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (Settings)
	{
		Settings->SetShadowQuality(DefaultIndex);
		OnShadowQualityUpdated();
	}

	GetActiveButton(DefaultIndex, ShadowQualityButtons);
	GetAndApplySettings();
}

void UCodeSettingsWidget::OnShadowQualityChanged(int32 Index)
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (Settings)
	{
		Settings->SetShadowQuality(Index);
		OnShadowQualityUpdated(); 
	}

	GetActiveButton(Index, ShadowQualityButtons);
	GetAndApplySettings();
}

void UCodeSettingsWidget::CheckTextureQuality()
{
	if (!SettingsMenuUI) return;

	UGameUserSettings* GameSettings = GEngine->GetGameUserSettings();
	if (!GameSettings) return;

	int32 CurrentTextureQuality = GameSettings->GetTextureQuality();

	if (CurrentTextureQuality == SettingsMenuUI->TextureQuality)
	{
		SettingsMenuUI->bIsSettingsSaved = true;
	}
	else
	{
		SettingsMenuUI->bIsSettingsSaved = false;
	}
}

void UCodeSettingsWidget::OnTextureQualityUpdated()
{
	CheckTextureQuality();

	for (int32 i = 0; i < TextureQualityButtons.Num(); ++i)
	{
		if (TextureQualityButtons[i])
		{
			TextureQualityButtons[i]->OnButtonClickedWithIndex.Clear();
			TextureQualityButtons[i]->OnButtonClickedWithIndex.AddDynamic(this, &UCodeSettingsWidget::OnTextureQualityChanged);
		}
	}
}

void UCodeSettingsWidget::OnTextureQualityChanged(int32 Index)
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (Settings)
	{
		Settings->SetTextureQuality(Index);
		OnTextureQualityUpdated();
	}

	GetActiveButton(Index, TextureQualityButtons);
	GetAndApplySettings();
}

void UCodeSettingsWidget::LoadActiveButton(const TArray<UCodeMenuButton*>& InButtons, int32 Index)
{
	for (int32 i = 0; i < InButtons.Num(); ++i)
	{
		if (InButtons[i])
		{
			InButtons[i]->ChangeButtonStatus(i == Index);
		}
	}
}