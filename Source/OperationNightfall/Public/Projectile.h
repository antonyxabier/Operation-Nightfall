// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "Projectile.generated.h"

class UNiagaraSystem;
class UParticleSystem;

UCLASS()
class OPERATIONNIGHTFALL_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float DamageAmount;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* SphereCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* SphereMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float DestroyTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	FVector Scale;
	

	// Multiplier for headshot damage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float HeadshotMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float BodyShotMultiplier;



	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
	UNiagaraSystem* BloodFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TObjectPtr<USoundBase> metalSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TArray<USoundBase*> HurtSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects|Impact")
	UParticleSystem* MetalHitEffect;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
