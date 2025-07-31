// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeMenuPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "CodeMyController.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "GameFramework/PlayerController.h"

void ACodeMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();

    //APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    //if (PC)
    //{
        //PC->bShowMouseCursor = true;
       // PC->SetInputMode(FInputModeUIOnly());

        if (MainMenuWidgetClass)
        {
            MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
            if (MainMenuWidget)
            {
                MainMenuWidget->AddToViewport();

                FInputModeUIOnly IM;

                IM.SetWidgetToFocus(MainMenuWidget->TakeWidget());
                IM.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
                SetInputMode(IM);

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
//}

void ACodeMenuPlayerController::StopMainMenuMusic()
{
    if (MainMenuMusicComponent && MainMenuMusicComponent->IsPlaying())
    {
        MainMenuMusicComponent->Stop();
        MainMenuMusicComponent = nullptr;
    }
}

ACodeMenuPlayerController::ACodeMenuPlayerController()
{
    PrimaryActorTick.bCanEverTick = false;
    bShowMouseCursor = true;
}

void ACodeMenuPlayerController::EnsureMainMenuMusicPlaying()
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


