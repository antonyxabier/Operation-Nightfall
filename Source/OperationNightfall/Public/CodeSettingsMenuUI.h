// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CodeMenuButton.h"
#include "Blueprint/UserWidget.h"
#include "CodeSettingsMenuUI.generated.h"

/**
 * 
 */
class UButton;
class UWidgetSwitcher;
class UCodeSettingsWidget;

UCLASS()
class OPERATIONNIGHTFALL_API UCodeSettingsMenuUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> ButtonSave;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> ButtonReset;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> ButtonBack;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> ButtonDontApply;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> ButtonSaveChanges;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuClass;

	UFUNCTION()
	void HandleSaveClicked();

	UFUNCTION()
	void HandleBackClicked();

	UFUNCTION()
	void HandleResetClicked();

	UFUNCTION()
	void HandleDontApplyClicked();

	UFUNCTION()
	void HandleSaveChangesClicked();

	UFUNCTION()
	void HandlePauseInput();

	void ApplySettings();
	void SaveSettings();
	void LoadSettings();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	bool ApplyChanges(bool Condition);


public:
	// Settings values
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	int32 ShadowQuality;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	int32 TextureQuality;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	bool bIsSettingsSaved;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bOpenedFromPauseMenu = false;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetShadowQualityButton(int32 InShadowQuality);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeSettingsWidget> WB_Settings;

	UPROPERTY(meta = (BindWidget))
	class UBorder* BorderColor;
};
