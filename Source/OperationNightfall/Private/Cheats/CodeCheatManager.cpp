#include "Cheats/CodeCheatManager.h"
#include "BasePlayer.h"
#include "AC_HealthComponent.h"
#include "CodeBaseEnemy.h"
#include "CodeDamagePickup.h"
#include "CodeHealthPickup.h"
#include "GameFramework/SpectatorPawn.h"

void UCodeCheatManager::GodMode() {
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	ABasePlayer* Player = Cast<ABasePlayer>(PC->GetPawn());
	if (Player) {
		Player->bGodMode = true;

		UAC_HealthComponent* HC = Player->FindComponentByClass<UAC_HealthComponent>();
		if (HC) {
			HC->RestoreFullHealth();
			Player->UpdateHUD(1);
		}

		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Orange, TEXT("God Mode Activated!"));
	}
}

void UCodeCheatManager::SpawnAnItem() {
	// just a debug statement that lists available items to spawn
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue,
		TEXT("Available Items to Spawn:\n0 - Enemy\n1 - Mine\n2 - First Aid\n3 - Ammo\n"));
}

void UCodeCheatManager::SpawnItem(int ID) {
	// uses an int to spawn the appropriate items
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	ABasePlayer* Player = Cast<ABasePlayer>(PC->GetPawn());

	FVector SpawnLocation = Player->GetActorLocation() + Player->GetActorForwardVector() * 200;

	TSubclassOf<AActor> ItemToSpawn = nullptr;

	if (ID == 0) {
		ItemToSpawn = EnemyClass;
	}
	if (ID == 1) {
		ItemToSpawn = MineClass;
		SpawnLocation.Z -= 90;
	}
	if (ID == 2) {
		ItemToSpawn = FirstAidClass;
		SpawnLocation.Z -= 90;
	}
	if (ID == 3) {
		ItemToSpawn = AmmoClass;
		SpawnLocation.Z -= 30;
	}

	if (ItemToSpawn) {
		GetWorld()->SpawnActor<AActor>(ItemToSpawn, SpawnLocation, FRotator::ZeroRotator);
	}
}

void UCodeCheatManager::ToggleDebugCam() {
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	if (!bIsDebugActive) {
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Orange, TEXT("Enabling debug camera..."));
		PlayerPawn = PC->GetPawn();
		FVector SpawnLocation = PlayerPawn->GetActorLocation() + PlayerPawn->GetActorForwardVector() * 200;
		FRotator SpawnRotation = PlayerPawn->GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = PC;
		SpawnParams.Instigator = PlayerPawn;

		DebugSpectator = GetWorld()->SpawnActor<ASpectatorPawn>(ASpectatorPawn::StaticClass(), SpawnLocation, SpawnRotation);
		if (DebugSpectator) {
			PC->UnPossess();
			PC->Possess(DebugSpectator);
			bIsDebugActive = true;
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Orange, TEXT("Disabling debug camera..."));
		if (DebugSpectator) {
			DebugSpectator->Destroy();
			DebugSpectator = nullptr;
		}
		
		if (PlayerPawn) {
			PC->Possess(PlayerPawn);
			bIsDebugActive = false;
		}
	}
}

void UCodeCheatManager::ToggleFPS() {
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !FPSWidgetClass) return;

	if (!bIsFPSActive) {
		FPSWidget = CreateWidget<UFPSWidget>(PC, FPSWidgetClass);
		if (FPSWidget) {
			FPSWidget->AddToViewport();
			bIsFPSActive = true;
		}
	}
	else {
		FPSWidget->RemoveFromParent();
		FPSWidget = nullptr;
		bIsFPSActive = false;
	}
}