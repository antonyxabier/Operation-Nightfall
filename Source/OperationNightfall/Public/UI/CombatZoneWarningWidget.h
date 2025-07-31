// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatZoneWarningWidget.generated.h"


class UTextBlock;
/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API UCombatZoneWarningWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetSecondsRemaining(float Seconds);

protected:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountdownText;

};
