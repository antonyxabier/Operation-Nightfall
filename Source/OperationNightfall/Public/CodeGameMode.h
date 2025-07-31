// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Sound/SoundBase.h"
#include "CodeGameMode.generated.h"

class UResultsWidget;
/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API ACodeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACodeGameMode();

	UFUNCTION(BlueprintCallable)
	void AddEnemy(AActor* EnemyActor);

	virtual void BeginPlay() override;

	UPROPERTY()
	class ABasePlayer* CurrentPlayer;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UResultsWidget> ResultsWidgetClass;

	UPROPERTY()
	UResultsWidget* ResultsWidgetObj;

	UPROPERTY()
	int CurrentEnemyCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TObjectPtr<USoundBase> winSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TObjectPtr<USoundBase> loseSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TObjectPtr<USoundBase> spawnSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TObjectPtr<USoundBase> windSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TObjectPtr<USoundBase> ambientSound;


private:
	int NumberOfEnemies;

	UFUNCTION(BlueprintCallable)
	void RemoveEnemy(AActor* DestroyedActor);

	UFUNCTION(BlueprintCallable)
	void RemovePlayer();
	
};
