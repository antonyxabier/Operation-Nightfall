// Fill out your copyright notice in the Description page of Project Settings.


#include "CodePauseMenuUI.h"
#include "Kismet/GameplayStatics.h"
#include "CodeSettingsMenuUI.h"
#include "CodeCreditsMenu.h"
#include "CodeSettingsMenuUI.h"
#include "Components/HorizontalBox.h"
#include "Components/SizeBox.h"
#include "GameFramework/PlayerController.h"

void UCodePauseMenuUI::NativeConstruct()
{
    Super::NativeConstruct();

    // Play pause menu music that works during pause
    if (!PauseMusicComponent && PauseMenuMusic)
    {
        PauseMusicComponent = UGameplayStatics::SpawnSound2D(this, PauseMenuMusic);
        if (PauseMusicComponent)
        {
            PauseMusicComponent->bIsUISound = true;
            PauseMusicComponent->Play();
        }
    }

    if (Button_Resume) Button_Resume->OnButtonClicked.AddDynamic(this, &UCodePauseMenuUI::OnResumeClicked);
    if (Button_Settings) Button_Settings->OnButtonClicked.AddDynamic(this, &UCodePauseMenuUI::OnSettingsClicked);
    if (Button_Credits) Button_Credits->OnButtonClicked.AddDynamic(this, &UCodePauseMenuUI::OnCreditsClicked);
    if (Button_Quit) Button_Quit->OnButtonClicked.AddDynamic(this, &UCodePauseMenuUI::OnQuitClicked);

    if (ButtonMenu) ButtonMenu->OnButtonClicked.AddDynamic(this, &UCodePauseMenuUI::OnMainMenuClicked);
    if (ButtonDesktop) ButtonDesktop->OnButtonClicked.AddDynamic(this, &UCodePauseMenuUI::OnDesktopClicked);
}

void UCodePauseMenuUI::OnResumeClicked()
{
    if (PauseMusicComponent && PauseMusicComponent->IsValidLowLevel())
    {
        PauseMusicComponent->FadeOut(1.0f, 0.0f);
        PauseMusicComponent = nullptr;
    }

    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetPause(false);
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }

    RemoveFromParent();
}

void UCodePauseMenuUI::OnSettingsClicked()
{
    if (PauseMusicComponent && PauseMusicComponent->IsValidLowLevel())
    {
        PauseMusicComponent->FadeOut(1.0f, 0.0f);
        PauseMusicComponent = nullptr;
    }

    if (!SettingsMenuInstance && SettingsMenuClass)
    {
        SettingsMenuInstance = CreateWidget<UCodeSettingsMenuUI>(GetOwningPlayer(), SettingsMenuClass);
        SettingsMenuInstance->bOpenedFromPauseMenu = true;
    }

    if (SettingsMenuInstance && !SettingsMenuInstance->IsInViewport())
    {
        SettingsMenuInstance->AddToViewport(1);
    }
}

void UCodePauseMenuUI::OnCreditsClicked()
{
    if (PauseMusicComponent && PauseMusicComponent->IsValidLowLevel())
    {
        PauseMusicComponent->FadeOut(1.0f, 0.0f);
        PauseMusicComponent = nullptr;
    }

    RemoveFromParent();

    if (!CreditsMenuInstance && CreditsMenuClass)
    {
        CreditsMenuInstance = CreateWidget<UCodeCreditsMenu>(GetOwningPlayer(), CreditsMenuClass);
    }

    if (CreditsMenuInstance && !CreditsMenuInstance->IsInViewport())
    {
        CreditsMenuInstance->bOpenedFromPauseMenu = true;
        CreditsMenuInstance->PreviousWidget = this;
        CreditsMenuInstance->AddToViewport();
    }
}

void UCodePauseMenuUI::OnQuitClicked()
{
    if (MainMenuDesktop)
    {
        MainMenuDesktop->SetVisibility(ESlateVisibility::Visible);
    }
}

void UCodePauseMenuUI::OnMainMenuClicked()
{
    UGameplayStatics::OpenLevel(this, FName("MainMenuMap"));
}

void UCodePauseMenuUI::OnDesktopClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}