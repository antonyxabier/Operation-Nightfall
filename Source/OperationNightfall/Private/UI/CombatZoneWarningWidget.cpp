// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CombatZoneWarningWidget.h"
#include "Components/TextBlock.h"




void UCombatZoneWarningWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCombatZoneWarningWidget::SetSecondsRemaining(float Seconds)
{
	if (CountdownText)
	{
		FString CountdownString = FString::Printf(TEXT("Return to Combat Zone: %.0f"), Seconds);
		CountdownText->SetText(FText::FromString(CountdownString));
	}
}