#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/ResultsWidget.h"
#include "BasePlayer.h"
#include "ONGameModeBase.generated.h"

UCLASS()
class OPERATIONNIGHTFALL_API AONGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AONGameModeBase();

	UFUNCTION()
	void AddEnemy(AActor* EnemyActor);
	UFUNCTION()
	void RemoveEnemy(AActor* NotUsed);

protected:
	virtual void BeginPlay() override;

	// widget referrences
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UResultsWidget> ResultsWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UResultsWidget* ResultsObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ABasePlayer* CurrPlayer;

	UFUNCTION()
	void RemovePlayer();

private:
	int EnemyCount = 0;
};
