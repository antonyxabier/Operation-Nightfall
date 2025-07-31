// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CodeMenuButton.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "CodeCreditsMenu.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API UCodeCreditsMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;
    virtual void NativePreConstruct() override;

    UPROPERTY()
    UUserWidget* PreviousWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Credits")
    bool bOpenedFromPauseMenu;

protected:
    UFUNCTION()
    void OnBackButtonClicked();

    UFUNCTION()
    void PlayCreditsAnimation();

    UFUNCTION()
    void OnPauseInputTriggered();

    UFUNCTION()
    void OnCreditsAnimationFinished();

    FTimerHandle CreditsTimerHandle;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TxtCredits;

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* CreditsRoll;

    UPROPERTY(meta = (BindWidget))
    UBorder* BorderColor;

    UPROPERTY(meta = (BindWidget))
    UCodeMenuButton* ButtonBack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PauseMenuClass;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* CreditsMusic;

    UPROPERTY()
    UAudioComponent* CreditsMusicComponent;

};
