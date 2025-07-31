// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "BaseCharacter.h"
#include "BasePlayer.h"    
#include "CodeBaseEnemy.h"
#include "CodeSpawner.h"


// Sets default values
AProjectile::AProjectile() : DestroyTimer(3.f)
{
	PrimaryActorTick.bCanEverTick = false;

	// Create and configure the Sphere Collision Component
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SetRootComponent(SphereCollision);

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetCollisionProfileName("NoCollision"); // Visual mesh should not block or generate hits
	SphereMesh->SetupAttachment(SphereCollision);
	

	SphereCollision->OnComponentHit.AddDynamic(this, &AProjectile::HandleHit); // Bind the OnHit event
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::HandleOverlap); // Bind the OnOverlap event
	


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	

	SphereCollision->SetWorldScale3D(Scale);
	DamageAmount = 1.0;
	HeadshotMultiplier = 5.0;
	BodyShotMultiplier = 1.0;
	BloodFX = nullptr;
	ImpactSound = nullptr;
	HurtSounds.Init(nullptr, 6);
	MetalHitEffect = nullptr;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereCollision->SetWorldScale3D(Scale);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AProjectile::K2_DestroyActor, DestroyTimer);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile overlapped with: %s"), *OtherActor->GetName());

	// Only proceed if we hit a valid actor and it's not ourselves or our owner
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
	{
		// Don't apply damage or friendly fire logic if it's self-overlap or invalid actor
		return;
	}

	// Get the instigator of this projectile (the pawn that fired it)
	APawn* ProjectileInstigator = GetInstigator();

	// --- FRIENDLY FIRE PREVENTION LOGIC ---
	// Check if both the instigator and the hit actor are enemies (ACodeBaseEnemy)
	ACodeBaseEnemy* InstigatorAsEnemy = Cast<ACodeBaseEnemy>(ProjectileInstigator);
	ACodeBaseEnemy* HitActorAsEnemy = Cast<ACodeBaseEnemy>(OtherActor);

	// If both are enemies, do NOT apply damage (friendly fire)
	if (InstigatorAsEnemy && HitActorAsEnemy)
	{
		UE_LOG(LogTemp, Log, TEXT("Projectile: Friendly fire prevented between enemies %s and %s."), *ProjectileInstigator->GetName(), *OtherActor->GetName());
		Destroy(); // Destroy projectile on friendly hit
		return; // Do not apply damage
	}

	// Check if the instigator is the player and the hit actor is the player (self-damage fallback)
	ABasePlayer* InstigatorAsPlayer = Cast<ABasePlayer>(ProjectileInstigator);
	if (InstigatorAsPlayer && OtherActor == InstigatorAsPlayer)
	{
		UE_LOG(LogTemp, Log, TEXT("Projectile: Player self-damage prevented (overlap fallback)."));
		Destroy(); // Destroy projectile on self-hit
		return;
	}

	// Check if the hit actor is the player and god mode is active
	ABasePlayer* HitPlayer = Cast<ABasePlayer>(OtherActor);
	if (HitPlayer && HitPlayer->bGodMode)
	{
		UE_LOG(LogTemp, Log, TEXT("Projectile: Hit player in God Mode. No damage applied."));
		Destroy(); // Destroy projectile even in God Mode
		return;
	}

	// --- PRECISION SHOT LOGIC ---
	float FinalDamageToApply = DamageAmount; // Start with base damage
	TSubclassOf<UDamageType> DamageTypeClass = UDamageType::StaticClass(); // Default damage type

	// Check if the hit was a headshot
	// You'll need to know the bone names of your character's head. Common names are "head", "Head", "neck_01", etc.
	// It's best to check your character's skeletal mesh in Unreal Editor for exact bone names.
	FName HitBoneName = SweepResult.BoneName; // Use SweepResult.BoneName for overlap
	if (HitBoneName == "head" || HitBoneName.ToString().Contains("head") || HitBoneName.ToString().Contains("neck_01")) 
	{
		FinalDamageToApply *= HeadshotMultiplier;
		// You can also use a specific DamageTypeClass here if you create BP_DT_Headshot
		UE_LOG(LogTemp, Log, TEXT("Headshot detected on %s! Final Damage: %f"), *OtherActor->GetName(), FinalDamageToApply);
	}
	else // Body shot or other non-head critical hit
	{
		FinalDamageToApply *= BodyShotMultiplier; // Apply body shot multiplier
		// You can also use a specific DamageTypeClass here if you create BP_DT_BodyShot
		UE_LOG(LogTemp, Log, TEXT("Body shot detected on %s! Final Damage: %f"), *OtherActor->GetName(), FinalDamageToApply);
	}

	// Spawn Blood FX if applicable
	if (BloodFX && Cast<ABaseCharacter>(OtherActor)) // Check if BloodFX is set and OtherActor is a Character
	{
		UGameplayStatics::PlaySound2D(this, ImpactSound);
		int32 RandomIndex = FMath::RandRange(0, HurtSounds.Num() - 1);
		UGameplayStatics::PlaySound2D(this, HurtSounds[RandomIndex]);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			BloodFX,
			SweepResult.ImpactPoint, // Use the impact point for precise FX location
			SweepResult.ImpactNormal.Rotation(), // Orient FX based on impact normal
			FVector(1.0f), // Scale of the effect (adjust as needed)
			true,          // Auto-destroy
			true,          // Auto-activate
			ENCPoolMethod::None,
			true
		);
		UE_LOG(LogTemp, Log, TEXT("Blood FX spawned on overlap at %s"), *SweepResult.ImpactPoint.ToString());
	}
	if (MetalHitEffect && Cast<ACodeSpawner>(OtherActor)) {
		UGameplayStatics::PlaySound2D(this, metalSound);
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			MetalHitEffect,
			SweepResult.ImpactPoint,
			SweepResult.ImpactNormal.Rotation(), // Orient the effect based on the surface normal
			FVector(1.0f), // Scale (adjust as needed)
			true // Auto-destroy
		);
	}
	
	// Apply damage to the hit actor
	UGameplayStatics::ApplyDamage(OtherActor, FinalDamageToApply, GetInstigatorController(), this, DamageTypeClass);
	UE_LOG(LogTemp, Log, TEXT("Projectile: Applied %f damage to %s. Instigator: %s"), FinalDamageToApply, *OtherActor->GetName(), ProjectileInstigator ? *ProjectileInstigator->GetName() : TEXT("None"));
	
	Destroy(); // Destroy the projectile after applying damage
}

void AProjectile::HandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	
}
