// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeMyController.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundBase.h"
#include "CodeMainMenuUI.h"
#include "CodeMainMenuLevelMap.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void ACodeMyController::BeginPlay()
{
    Super::BeginPlay();
    bShowMouseCursor = false;
    SetInputMode(FInputModeGameOnly());
    if (PlayerHUDClass) // PlayerHUDClass is a TSubclassOf<UUserWidget>
    {
        PlayerHUD = CreateWidget<UUserWidget>(this, PlayerHUDClass);
        if (PlayerHUD)
        {
            PlayerHUD->AddToViewport();
            UE_LOG(LogTemp, Log, TEXT("PlayerHUD added to viewport."));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create PlayerHUD widget."));
        }
    }
    

    if (!PauseMenuUI && PauseMenuClass)
    {
        PauseMenuUI = CreateWidget<UUserWidget>(this, PauseMenuClass);
    }
}

void ACodeMyController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("PauseGame", IE_Pressed, this, &ACodeMyController::TogglePauseMenuUI);
}

void ACodeMyController::TogglePauseMenuUI()
{
    if (!PauseMenuUI && PauseMenuClass)
    {
        PauseMenuUI = CreateWidget<UUserWidget>(this, PauseMenuClass);
    }

    if (PauseMenuUI)
    {
        if (PauseMenuUI->IsInViewport())
        {
            PauseMenuUI->RemoveFromParent();
            bShowMouseCursor = false;
            SetInputMode(FInputModeGameOnly());
            UGameplayStatics::SetGamePaused(this, false);

            if (PauseMenuAudioComponent)
            {
                PauseMenuAudioComponent->Stop();
            }
        }
        else
        {
            PauseMenuUI->AddToViewport();
            bShowMouseCursor = true;

            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(PauseMenuUI->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            SetInputMode(InputMode);

            UGameplayStatics::SetGamePaused(this, true);

            if (PauseMenuMusic)
            {
                PauseMenuAudioComponent = UGameplayStatics::SpawnSound2D(this, PauseMenuMusic);
                if (PauseMenuAudioComponent)
                {
                    PauseMenuAudioComponent->bIsUISound = true;
                }
            }
        }
    }
}

void ACodeMyController::PlayMainMenuMusic()
{
    if (!MainMenuMusicComponent && MainMenuMusic)
    {
        MainMenuMusicComponent = UGameplayStatics::SpawnSound2D(this, MainMenuMusic);
    }
}

void ACodeMyController::StopMainMenuMusic()
{
    if (MainMenuMusicComponent && MainMenuMusicComponent->IsPlaying())
    {
        MainMenuMusicComponent->FadeOut(1.0f, 0.0f);
        MainMenuMusicComponent = nullptr;
    }
}