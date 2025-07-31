// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeGameMode.h"
#include "BasePlayer.h"
#include "CodeMenuPlayerController.h"
#include "UI/ResultsWidget.h"
#include "EngineUtils.h"
#include "CodeSpawner.h"
#include <Kismet/GameplayStatics.h>

ACodeGameMode::ACodeGameMode() :NumberOfEnemies(0), CurrentPlayer(nullptr), CurrentEnemyCount(0)
{
	PlayerControllerClass = ACodeMenuPlayerController::StaticClass();
	DefaultPawnClass = nullptr;


}

void ACodeGameMode::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::PlaySound2D(this, windSound);
	UGameplayStatics::PlaySound2D(this, ambientSound);
	for (TActorIterator<ABaseCharacter> It(GetWorld()); It; ++It)
	{
		ABaseCharacter* Character = *It;
		if (Character)
		{
			if (Character->IsA<ABasePlayer>())
			{
				CurrentPlayer = Cast<ABasePlayer>(Character);
				CurrentPlayer->OnPlayerLost.AddDynamic(this, &ACodeGameMode::RemovePlayer);
			}
			else
			{
				AddEnemy(Character);
			}
		}
	}
	for (TActorIterator<ACodeSpawner> It(GetWorld()); It; ++It)
	{
		ACodeSpawner* Spawner = *It;
		if (Spawner)
		{
			AddEnemy(Spawner);
			UGameplayStatics::PlaySound2D(this, spawnSound);
		}
	}
	if (ResultsWidgetClass)
	{
		ResultsWidgetObj = CreateWidget<UResultsWidget>(GetWorld(), ResultsWidgetClass);
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("ResultsWidgetClass is NULL!"));
	}
}

void ACodeGameMode::AddEnemy(AActor* EnemyActor)
{
	EnemyActor->OnDestroyed.AddDynamic(this, &ACodeGameMode::RemoveEnemy);
	NumberOfEnemies++;
	UE_LOG(LogTemp, Error, TEXT("enemy added | enemy count: %d"), NumberOfEnemies);
}

void ACodeGameMode::RemoveEnemy(AActor* DestroyedActor)
{
	NumberOfEnemies--;
	if (NumberOfEnemies <= 0) {
		GEngine->AddOnScreenDebugMessage(-1, 12.0f, FColor::Green, TEXT("YOU WIN!"));
		if (ResultsWidgetObj)
		{
			ResultsWidgetObj->AddToViewport();
			ResultsWidgetObj->SetWin();
		}
		if (CurrentPlayer) {
			CurrentPlayer->PlayerWin();
			
			UGameplayStatics::PlaySound2D(this, winSound);
		}
	}
}

void ACodeGameMode::RemovePlayer()
{
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, TEXT("YOU LOSE!"));
	UGameplayStatics::PlaySound2D(this, loseSound);

	if (ResultsWidgetObj)
	{
		// Show widget on screen
		ResultsWidgetObj->AddToViewport();

		// Optionally tell the widget it’s a loss
		//ResultsWidgetObj->SetLose(); // Assuming you implemented this in CodeResultsWidget

		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			// Switch to UI-only input
			FInputModeUIOnly InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetWidgetToFocus(ResultsWidgetObj->TakeWidget());

			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
		}
	}

	// Null out player reference (if needed)
	CurrentPlayer = nullptr;
}



