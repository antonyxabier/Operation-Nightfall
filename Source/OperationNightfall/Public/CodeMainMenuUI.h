// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CodeMainMenuUI.generated.h"

/**
 * 
 */

class UCodeMenuButton;

UCLASS()
class OPERATIONNIGHTFALL_API UCodeMainMenuUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

	// Button references 
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> Button_Play;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> Button_Quit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> CreditsMenuClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> Button_Credits;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> Button_Settings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> SettingsMenuClass;

	UFUNCTION()
	void HandleSettingsClicked();


	UFUNCTION()
	void HandleCreditsClicked();

	// Button handlers
	UFUNCTION()
	void HandlePlayClicked();

	UFUNCTION()
	void HandleQuitClicked();

};
