#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "UI/FPSWidget.h"
#include "CodeCheatManager.generated.h"

UCLASS()
class OPERATIONNIGHTFALL_API UCodeCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
	UFUNCTION(exec) // 'exec' tells unreal the funciton is only callable from in-game console
	void GodMode();

	UFUNCTION(exec)
	void SpawnAnItem();

	UFUNCTION(exec)
	void SpawnItem(int ID);

	UFUNCTION(exec)
	void ToggleDebugCam();

	UFUNCTION(exec)
	void ToggleFPS();

protected:
	// item references
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> MineClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> FirstAidClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> AmmoClass;

private:
	// camera components
	APawn* PlayerPawn = nullptr;
	ASpectatorPawn* DebugSpectator = nullptr;
	bool bIsDebugActive = false;

	// FPS components
	bool bIsFPSActive = false;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFPSWidget> FPSWidgetClass;
	UUserWidget* FPSWidget;
};
