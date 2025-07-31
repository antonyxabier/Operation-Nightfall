// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CodeMyController.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API ACodeMyController : public APlayerController
{
	GENERATED_BODY()
	
public:
    UUserWidget* GetPauseMenuWidget() const { return PauseMenuUI; }

    UPROPERTY()
    UAudioComponent* MainMenuMusicComponent;

    UFUNCTION(BlueprintCallable)
    void PlayMainMenuMusic();

    UFUNCTION(BlueprintCallable)
    void StopMainMenuMusic();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> PlayerHUDClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> PauseMenuClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> ControlsClass;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* PauseMenuMusic;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* MainMenuMusic;

private:
    UPROPERTY()
    UUserWidget* PlayerHUD;

    UPROPERTY()
    UUserWidget* PauseMenuUI;

    UPROPERTY()
    UUserWidget* ControlsWidget;

    bool bIsPaused = false;

    UFUNCTION()
    void TogglePauseMenuUI();

    UPROPERTY()
    UAudioComponent* PauseMenuAudioComponent;
};
