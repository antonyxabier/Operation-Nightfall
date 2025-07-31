// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "Engine/LevelScriptActor.h"
#include "CodeMainMenuLevelMap.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API ACodeMainMenuLevelMap : public ALevelScriptActor
{
	GENERATED_BODY()
	
protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;

    UPROPERTY()
    UAudioComponent* MainMenuMusicComponent;

private:
    UPROPERTY()
    UUserWidget* MainMenuWidget;

public:
    UFUNCTION(BlueprintCallable)
    void StopMainMenuMusic();

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundBase* MainMenuMusic;

    UFUNCTION(BlueprintCallable)
    void EnsureMainMenuMusicPlaying();
};
