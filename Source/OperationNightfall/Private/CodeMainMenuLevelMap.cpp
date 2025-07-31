// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeMainMenuLevelMap.h"
#include "Blueprint/UserWidget.h"
#include "CodeMyController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "GameFramework/PlayerController.h"

void ACodeMainMenuLevelMap::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeUIOnly());

        if (MainMenuWidgetClass)
        {
            MainMenuWidget = CreateWidget<UUserWidget>(PC, MainMenuWidgetClass);
            if (MainMenuWidget)
            {
                MainMenuWidget->AddToViewport();
            }
        }

        if (MainMenuMusic)
        {
            MainMenuMusicComponent = UGameplayStatics::SpawnSound2D(this, MainMenuMusic);
            if (MainMenuMusicComponent)
            {
                MainMenuMusicComponent->bIsUISound = true;
            }
        }
    }
}

void ACodeMainMenuLevelMap::StopMainMenuMusic()
{
    if (MainMenuMusicComponent && MainMenuMusicComponent->IsPlaying())
    {
        MainMenuMusicComponent->Stop();
        MainMenuMusicComponent = nullptr;
    }
}

void ACodeMainMenuLevelMap::EnsureMainMenuMusicPlaying()
{
    if (!MainMenuMusicComponent && MainMenuMusic)
    {
        MainMenuMusicComponent = UGameplayStatics::SpawnSound2D(this, MainMenuMusic, 1.0f);
    }
    else if (MainMenuMusicComponent && !MainMenuMusicComponent->IsPlaying())
    {
        MainMenuMusicComponent = UGameplayStatics::SpawnSound2D(this, MainMenuMusic, 1.0f);
    }
}
