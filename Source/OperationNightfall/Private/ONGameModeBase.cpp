#include "ONGameModeBase.h"
#include "EngineUtils.h"

AONGameModeBase::AONGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AONGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	CurrPlayer = nullptr;

	for (TActorIterator<ABaseCharacter> itr(GetWorld()); itr; ++itr) {
		ABaseCharacter* Character = *itr;

		if (ABasePlayer* Player = Cast<ABasePlayer>(Character)) {
			CurrPlayer = Player;
			CurrPlayer->OnPlayerLost.AddDynamic(this, &AONGameModeBase::RemovePlayer);
		}
		else {
			AddEnemy(Character);
		}
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	ResultsObject = CreateWidget<UResultsWidget>(PC, ResultsWidget);
}

void AONGameModeBase::AddEnemy(AActor* EnemyActor) {
	EnemyActor->OnDestroyed.AddDynamic(this, &AONGameModeBase::RemoveEnemy);
	++EnemyCount;
	UE_LOG(LogTemp, Error, TEXT("enemy added | enemy count: %d"), EnemyCount);
}

void AONGameModeBase::RemoveEnemy(AActor* NotUsed) {
	--EnemyCount;

	if (EnemyCount <= 0) {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("YOU WIN!"));
		ResultsObject->AddToViewport();
		CurrPlayer->PlayerWin();
		ResultsObject->SetWin();
	}
}

void AONGameModeBase::RemovePlayer() {
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("YOU LOSE!!"));
	ResultsObject->AddToViewport();
}