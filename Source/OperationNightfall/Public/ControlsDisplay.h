// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ControlsDisplay.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API UControlsDisplay : public UUserWidget
{
	GENERATED_BODY()

public:

	//moUControlsDisplay();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls Text")
	UTextBlock* MoveForwardText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls Text")
	UTextBlock* MoveStrafeText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls Text")
	UTextBlock* TurnText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls Text")
	UTextBlock* LookText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls Text")
	UTextBlock* FireText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Objective")
	UTextBlock* CurrentObjectiveText;
	
};
