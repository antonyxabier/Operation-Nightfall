// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeSpawner.h"
#include "CodeBaseEnemy.h"
#include "CodeGameMode.h"
#include "AC_HealthComponent.h"
#include "Components/SceneComponent.h"
#include <Kismet/GameplayStatics.h>
#include "NiagaraFunctionLibrary.h"



// Sets default values
ACodeSpawner::ACodeSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SpawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerMesh"));
	SpawnerMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	SpawnerMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
	SpawnerMesh->SetCollisionObjectType(ECC_Pawn);
	SpawnerMesh->SetupAttachment(RootComponent);
	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(SpawnerMesh); // Attach to the truck mesh
	
	SpawnPoint->SetRelativeLocation(FVector(-200.f, 0.f, 50.f)); 
	SpawnPoint->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	HealthComponent = CreateDefaultSubobject<UAC_HealthComponent>("HealthComponent");

	AgentToSpawn = nullptr; 
	SpawnRate = 2.0f; 
	MaxSpawns = 10;   
	SpawnedCount = 0; 
	ExplosionFX = nullptr;

}

// Called when the game starts or when spawned
void ACodeSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (AgentToSpawn && MaxSpawns > 0)
	{
		// Start the timer to call SpawnAgent repeatedly
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ACodeSpawner::SpawnAgent, SpawnRate, true);
	}
	HealthComponent->OnDeath.AddDynamic(this, &ACodeSpawner::OnSpawnerDeath);
	
}

// Called every frame
void ACodeSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACodeSpawner::SpawnAgent()
{
	if (SpawnedCount >= MaxSpawns)
	{
		// All agents spawned, clear the timer
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Spawning Agent... (Count: %d / %d)"), SpawnedCount + 1, MaxSpawns);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Spawning Agent... (Count: %d / %d)"), SpawnedCount + 1, MaxSpawns));

	// Make sure AgentToSpawn is valid before trying to spawn
	if (!AgentToSpawn)
	{
		UE_LOG(LogTemp, Error, TEXT("Spawner %s: AgentToSpawn is null! Cannot spawn agent."), *GetName());
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle); // Stop timer if invalid asset
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; // Spawner owns the agent
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	// Spawn location (adjust 75.f based on your agent's pivot for ground placement)
	FVector SpawnLocation = SpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = SpawnPoint->GetComponentRotation();

	ACodeBaseEnemy* SpawnedAgent = GetWorld()->SpawnActor<ACodeBaseEnemy>(AgentToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

	if (SpawnedAgent)
	{
		SpawnedCount++; // Increment count only if spawn was successful
		UE_LOG(LogTemp, Log, TEXT("Spawned agent: %s"), *SpawnedAgent->GetName());

		// Removed GameMode interaction as requested
		ACodeGameMode* GM = Cast<ACodeGameMode>(GetWorld()->GetAuthGameMode());
		 if (GM)
		 {
		 	GM->AddEnemy(SpawnedAgent);
		 }
		 else
		 {
		 	UE_LOG(LogTemp, Error, TEXT("Failed to cast GameMode to ACodeGameMode. Cannot add enemy."));
		 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to cast GameMode to ACodeGameMode"));
		 }
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn agent for spawner %s!"), *GetName());
		// If spawning fails, you might want to break the timer to prevent infinite attempts
		// GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	}

}


void ACodeSpawner::OnSpawnerDeath(float ratio)
{
	UE_LOG(LogTemp, Log, TEXT("Spawner is dead!"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Spawner is dead!"));

	if (ExplosionFX && GetWorld()) // Ensure ExplosionFX and World are valid
	{
		UGameplayStatics::PlaySound2D(this, ExplosionSound);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ExplosionFX,
			GetActorLocation(), // Spawn at the spawner's location
			GetActorRotation(), // Use spawner's rotation (or FRotator::ZeroRotator)
			FVector(1.0f),      // Scale of the effect (adjust as needed)
			true,               // Auto-destroy the FX when it's done
			true,               // Required for network replication if multiplayer
			ENCPoolMethod::None,
			true
		);
		UE_LOG(LogTemp, Log, TEXT("Explosion FX spawned for spawner: %s"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawner %s: ExplosionFX is null or World is invalid. Cannot spawn explosion effect."), *GetName());
	}

	Destroy();
}



