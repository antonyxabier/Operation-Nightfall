// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CodeMenuButton.h"
#include "Components/WidgetSwitcher.h"
#include "CodeSettingsWidget.generated.h"

/**
 * 
 */
class UBorder;

UCLASS()
class OPERATIONNIGHTFALL_API UCodeSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public: 
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void LoadActiveButton(const TArray<UCodeMenuButton*>& Buttons, int32 ActiveIndex);

	UPROPERTY(BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UCodeMenuButton>> ShadowQualityButtons;

	UPROPERTY(BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UCodeMenuButton>> TextureQualityButtons;
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleGraphicsClicked();

	UFUNCTION()
	void HandleControlsClicked();

	UFUNCTION()
	void HandleAudioClicked();

	// Navigation buttons
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> Graphics;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> Audio;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> Controls;

	// Widget Switcher
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher_Settings;

	// Shadow quality buttons
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> ShadowQuality00;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> ShadowQuality01;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> ShadowQuality02;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> ShadowQuality03;

	// Texture quality buttons
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> TextureQuality00;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> TextureQuality01;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> TextureQuality02;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCodeMenuButton> TextureQuality03;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<class UCodeSettingsMenuUI> SettingsMenuUI;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void CheckShadowQuality();

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetDefaultShadowQuality();

	UCodeMenuButton* GetActiveButton(int32 Index, TArray<TObjectPtr<UCodeMenuButton>>& TargetArray);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void GetAndApplySettings();

	UFUNCTION()
	void OnShadowQualityUpdated();

	UFUNCTION()
	void OnShadowQualityChanged(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void CheckTextureQuality();

	UFUNCTION()
	void OnTextureQualityUpdated();

	UFUNCTION()
	void OnTextureQualityChanged(int32 Index);


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> BorderColor;
};
