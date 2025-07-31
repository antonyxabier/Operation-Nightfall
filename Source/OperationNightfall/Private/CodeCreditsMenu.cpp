// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeCreditsMenu.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "CodeMyController.h"
#include "CodeMainMenuLevelMap.h"
#include "CodeMainMenuUI.h"
#include "Components/AudioComponent.h"
#include "Animation/UMGSequencePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Animation/WidgetAnimation.h"
#include "GameFramework/PlayerController.h"

void UCodeCreditsMenu::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (CreditsMusic)
    {
        CreditsMusicComponent = UGameplayStatics::SpawnSound2D(this, CreditsMusic);
    }

    if (BorderColor)
    {
        FLinearColor Transparent = FLinearColor::White;
        Transparent.A = 0.0f;

        FLinearColor BrushColor = UGameplayStatics::IsGamePaused(GetWorld()) ? Transparent : FLinearColor::White;
        BorderColor->SetBrushColor(BrushColor);
    }
}

void UCodeCreditsMenu::NativeConstruct()
{
    Super::NativeConstruct();

    // Play animation
    PlayCreditsAnimation();

    // Bind button click
    if (ButtonBack)
    {
        ButtonBack->OnButtonClicked.AddDynamic(this, &UCodeCreditsMenu::OnBackButtonClicked);
    }

    // Bind input action for pause (like Escape)
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (PC->InputComponent)
        {
            FInputActionBinding& Bind = PC->InputComponent->BindAction("PauseGame", IE_Pressed, this, &UCodeCreditsMenu::OnPauseInputTriggered);
            Bind.bConsumeInput = true;
        }
    }
}

void UCodeCreditsMenu::PlayCreditsAnimation()
{
    if (CreditsRoll)
    {
        PlayAnimation(CreditsRoll);

        //PlayAnimation(CreditsRoll, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, false);

        //float Duration = CreditsRoll->GetEndTime();
        //if (Duration > 0.0f)
        //{
        //    GetWorld()->GetTimerManager().SetTimer(
        //        CreditsTimerHandle,
        //        this,
        //        &UCodeCreditsMenu::OnCreditsAnimationFinished,
        //        Duration,
        //        false
        //    );
        //}
    }
}

void UCodeCreditsMenu::OnCreditsAnimationFinished()
{
   /* GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
        {
            TxtCredits->SetVisibility(ESlateVisibility::Hidden);
        });*/
}

void UCodeCreditsMenu::OnBackButtonClicked()
{
    if (CreditsMusicComponent && CreditsMusicComponent->IsPlaying())
    {
        CreditsMusicComponent->Stop();
        CreditsMusicComponent = nullptr;
    }

    RemoveFromParent(); // Remove this credits menu

    // Clear any pending animations or timers
    GetWorld()->GetTimerManager().ClearTimer(CreditsTimerHandle);

    if (bOpenedFromPauseMenu)
    {
        if (ACodeMyController* MyPC = Cast<ACodeMyController>(GetOwningPlayer()))
        {
            if (UUserWidget* PauseMenu = MyPC->GetPauseMenuWidget())
            {
                if (!PauseMenu->IsInViewport())
                {
                    PauseMenu->AddToViewport();
                }

                MyPC->bShowMouseCursor = true;

                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(PauseMenu->TakeWidget());
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                MyPC->SetInputMode(InputMode);

                return;
            }
        }
    }
    else if (MainMenuClass)
    {
        // fallback if opened from main menu
        if (UCodeMainMenuUI* Menu = CreateWidget<UCodeMainMenuUI>(GetOwningPlayer(), MainMenuClass))
        {
            Menu->AddToViewport();

            if (ACodeMainMenuLevelMap* LevelScript = Cast<ACodeMainMenuLevelMap>(GetWorld()->GetLevelScriptActor()))
            {
                LevelScript->EnsureMainMenuMusicPlaying();
            }

            if (APlayerController* PC = GetOwningPlayer())
            {
                PC->bShowMouseCursor = true;
                FInputModeUIOnly InputMode;
                InputMode.SetWidgetToFocus(Menu->TakeWidget());
                InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                PC->SetInputMode(InputMode);
            }
        }
    }
}

void UCodeCreditsMenu::OnPauseInputTriggered()
{
    RemoveFromParent();
}
