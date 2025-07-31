// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeMenuButton.h"
#include "Engine/Font.h"
#include "Fonts/SlateFontInfo.h"
#include <Kismet/GameplayStatics.h>

void UCodeMenuButton::NativePreConstruct()
{
    Super::NativePreConstruct();

	if (TxtButtonName)
	{
		TxtButtonName->SetText(ButtonName);

		FSlateFontInfo FontInfo = TxtButtonName->Font;
		FontInfo.Size = FontSize;
		FontInfo.TypefaceFontName = Typeface;
		TxtButtonName->SetFont(FontInfo);
	}
}

void UCodeMenuButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ButtonMenuUI)
	{
		// Bind the button events
		ButtonMenuUI->OnHovered.AddDynamic(this, &UCodeMenuButton::HandleHovered);
		ButtonMenuUI->OnUnhovered.AddDynamic(this, &UCodeMenuButton::HandleUnhovered);
		ButtonMenuUI->OnClicked.AddDynamic(this, &UCodeMenuButton::HandleClicked);

		// Set default color
		ButtonMenuUI->SetBackgroundColor(DefaultColor);
	}
}

void UCodeMenuButton::HandleHovered()
{
	if (ButtonMenuUI)
	{
		// set hover color
		ButtonMenuUI->SetBackgroundColor(HoveredColor);
	}

	if (HoverSound)
	{
		// set hover sound
		UGameplayStatics::PlaySound2D(this, HoverSound);
	}
}

void UCodeMenuButton::HandleUnhovered()
{
	if (ButtonMenuUI)
	{
		// play click sound
		ButtonMenuUI->SetBackgroundColor(DefaultColor);
	}
}

void UCodeMenuButton::HandleClicked()
{
	if (ClickSound)
	{
		// trigger delegate
		UGameplayStatics::PlaySound2D(this, ClickSound);
	}

	OnButtonClicked.Broadcast();
	if (Index >= 0)
	{
		OnSelected.Broadcast(Index);
		OnButtonClickedWithIndex.Broadcast(Index);
	}
}

void UCodeMenuButton::ChangeButtonStatus(bool bNewIsActive)
{
	bIsActive = bNewIsActive;

	if (TxtButtonName)
	{
		FSlateColor NewColor;
		if (bIsActive)
		{
			NewColor = FSlateColor(FLinearColor(0.0f, 1.0f, 0.0f, 1.0f));
		}
		else
		{
			NewColor = FSlateColor(FLinearColor::White);
		}

		TxtButtonName->SetColorAndOpacity(NewColor);
	}
}