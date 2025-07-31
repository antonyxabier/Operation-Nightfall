// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "CodeMenuButton.h"
#include "CodeCreditsMenu.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Blueprint/UserWidget.h"
#include "CodePauseMenuUI.generated.h"

/**
 * 
 */
class UCodeCreditsMenu;
class UCodeSettingsMenuUI;

UCLASS()
class OPERATIONNIGHTFALL_API UCodePauseMenuUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnResumeClicked();

    UFUNCTION()
    void OnSettingsClicked();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> CreditsMenuClass;

    UFUNCTION()
    void OnCreditsClicked();

    UFUNCTION()
    void OnQuitClicked();

    UFUNCTION()
    void OnMainMenuClicked();

    UFUNCTION()
    void OnDesktopClicked();

    UPROPERTY(meta = (BindWidget))
    UCodeMenuButton* Button_Resume;

    UPROPERTY(meta = (BindWidget))
    UCodeMenuButton* Button_Settings;

    UPROPERTY(meta = (BindWidget))
    UCodeMenuButton* Button_Credits;

    UPROPERTY(meta = (BindWidget))
    UCodeMenuButton* Button_Quit;

    UPROPERTY(meta = (BindWidget))
    class USizeBox* MainMenuDesktop;

    UPROPERTY(meta = (BindWidget))
    UCodeMenuButton* ButtonMenu;

    UPROPERTY(meta = (BindWidget))
    UCodeMenuButton* ButtonDesktop;

    // The audio component that will handle music playback
    UPROPERTY()
    UAudioComponent* PauseMusicComponent;

    // The music asset to play
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TObjectPtr<USoundBase> PauseMenuMusic;

    UPROPERTY()
    UCodeCreditsMenu* CreditsMenuInstance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> SettingsMenuClass;

    UPROPERTY()
    UCodeSettingsMenuUI* SettingsMenuInstance;
};
