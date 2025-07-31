// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CodeGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API UCodeGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UCodeGameInstance();

	UFUNCTION()
	void LoadFirstLevel();
	UFUNCTION()
	void LoadCurrentLevel();
	UFUNCTION()
	void LoadMainMenu();
	UFUNCTION()
	void ExitTheGame();


protected:



private:

	UFUNCTION(BlueprintCallable)
	void LoadLevelSafe(int LevelIndex);

	UPROPERTY(EditAnywhere, Category = GameLevels)
	TArray<FName> GameLevels;

	UPROPERTY(EditAnywhere, Category = GameLevels)
	int FirstLevelIndex;

	UPROPERTY(EditAnywhere, Category = GameLevels)
	int CurrentLevelIndex;
	
};
