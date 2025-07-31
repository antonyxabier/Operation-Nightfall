// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h" 
#include "Components/TextBlock.h"
#include "Sound/SoundBase.h"
#include "CodeMenuButton.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuButtonClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelected, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnButtonClickedWithIndex, int32, Index);

UCLASS()
class OPERATIONNIGHTFALL_API UCodeMenuButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

	// Click delegate
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMenuButtonClicked OnButtonClicked;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSelected OnSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (ExposeOnSpawn = true))
	int32 Index = -1;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ChangeButtonStatus(bool bIsActive);

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	bool bIsActive = false;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnButtonClickedWithIndex OnButtonClickedWithIndex;

protected:
	// UI elements
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TxtButtonName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonMenuUI;

	// Sounds for hover and click
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Sound")
	TObjectPtr<USoundBase> HoverSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Sound")
	TObjectPtr<USoundBase> ClickSound;

	// Background color changes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Visual")
	FLinearColor HoveredColor = FLinearColor(0.05f, 0.05f, 0.05f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Visual")
	FLinearColor DefaultColor = FLinearColor::Black;

	// label for the button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText ButtonName = FText::FromString("Menu");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 FontSize = 38;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FName Typeface = FName("Regular");

	// Internal callbacks
	UFUNCTION()
	void HandleHovered();

	UFUNCTION()
	void HandleUnhovered();

	UFUNCTION()
	void HandleClicked();

};
