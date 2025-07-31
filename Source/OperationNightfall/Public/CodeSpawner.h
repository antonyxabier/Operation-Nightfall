// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "CodeSpawner.generated.h"

class USceneComponent; 
class ACodeBaseEnemy;
class UAC_HealthComponent;
class UNiagaraSystem;

UCLASS()
class OPERATIONNIGHTFALL_API ACodeSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACodeSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Defaults)
	class UStaticMeshComponent* SpawnerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACodeBaseEnemy> AgentToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSpawns;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
	class UAC_HealthComponent* HealthComponent;

	UFUNCTION()
	void OnSpawnerDeath(float ratio);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	UNiagaraSystem* ExplosionFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TObjectPtr<USoundBase> ExplosionSound;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Internal counter for agents already spawned
	int32 SpawnedCount;

	// Timer handle for repetitive spawning
	FTimerHandle SpawnTimerHandle;

	// Function to handle the spawning of a single agent
	UFUNCTION() // UFUNCTION needed for SetTimer
	void SpawnAgent();


};
