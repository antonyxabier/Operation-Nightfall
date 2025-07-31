// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeGameInstance.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Engine/Engine.h"


UCodeGameInstance::UCodeGameInstance() : FirstLevelIndex(1)
{
    GameLevels.Add(FName("MainMenuMap"));
    GameLevels.Add(FName("Demo_Scene"));
}

void UCodeGameInstance::LoadFirstLevel()
{
    LoadLevelSafe(FirstLevelIndex);
}

void UCodeGameInstance::LoadCurrentLevel()
{
    LoadLevelSafe(CurrentLevelIndex);
}

void UCodeGameInstance::LoadMainMenu()
{
    LoadLevelSafe(0);
}

void UCodeGameInstance::ExitTheGame()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void UCodeGameInstance::LoadLevelSafe(int LevelIndex)
{

    if (!GameLevels.IsValidIndex(LevelIndex))
    {
        FString Error = FString::Printf(TEXT("ERROR: Level Index %d is not valid in the Game Instance"), LevelIndex);
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Error);
        }
        return;
    }
    CurrentLevelIndex = LevelIndex;
    FName LevelName = GameLevels[LevelIndex];
    UWorld* World = GetWorld();

    if (World)
    {
        // ServerTravel works for both single and multiplayer games
        FString LevelString = LevelName.ToString();
        World->ServerTravel(LevelString);
    }

}



