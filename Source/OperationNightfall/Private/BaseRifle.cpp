#include "BaseRifle.h"
#include "Projectile.h"
#include "Granade.h"
#include "BasePlayer.h"
#include <Kismet/KismetMathLibrary.h>

ABaseRifle::ABaseRifle()
{
 	PrimaryActorTick.bCanEverTick = true;

	// set up skeletal mesh
	baseRifleMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BaseRifleMesh");
	SetRootComponent(baseRifleMesh);

	// Set the collision response to block WorldStatic and WorldDynamic objects
	baseRifleMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // Start by ignoring all
	baseRifleMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block); // Block static environment
	baseRifleMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block); // Block dynamic environment
	baseRifleMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); // Overlap with pawns (player/enemy)

	// Set the projectile class
	ProjectileClass = AProjectile::StaticClass();
	GranadeClass = AGranade::StaticClass();
	// set up flashlight
	FlashlightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	FlashlightComponent->SetupAttachment(baseRifleMesh, WeaponSocketName); // Attach to the muzzle socket
	FlashlightComponent->SetRelativeLocation(FVector(20.0f, 0.0f, 0.0f)); // Adjust offset from muzzle
	FlashlightComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f)); // Point forward
	FlashlightComponent->Intensity = 5000.0f; // Adjust brightness
	FlashlightComponent->LightColor = FColor::White;
	FlashlightComponent->AttenuationRadius = 2000.0f; // Adjust range
	FlashlightComponent->OuterConeAngle = 45.0f; // Adjust cone width
	FlashlightComponent->InnerConeAngle = 30.0f;
	FlashlightComponent->bUseInverseSquaredFalloff = true; // Realistic falloff
	FlashlightComponent->CastShadows = true;
	FlashlightComponent->SetVisibility(false); // Start off
	bFlashlightOn = false; // Initial state
}

void ABaseRifle::BeginPlay()
{
	Super::BeginPlay();
	ParentPawn = Cast<APawn>(GetParentActor());
	if (ParentPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Parent Pawn: %s"), *ParentPawn->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Owner Must be A Pawn"));
		Destroy();
	}
	ReloadAmmo();
}

// unused
void ABaseRifle::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ABaseRifle::Attack()
{
	AProjectile* SpawnedProjectile = nullptr;

	FVector pos;
	FRotator rotator;

	ABasePlayer* Player = Cast<ABasePlayer>(ParentPawn);
	if (Player)
	{
		auto hudWidget = Player->GetHUDWidget();
		// Player-specific logic for setting rotation
		if (!hudWidget) return;

		UPlayerHUD* PlayerHUD = Cast<UPlayerHUD>(hudWidget);
		if (!PlayerHUD) return;

		FVector StartLocation = GetSource();  // Get rifle's firing position
		FVector EndLocation = PlayerHUD->GetDestination(); // Get crosshair destination

		// Calculate rotation from weapon to crosshair target
		rotator = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);
	}
	else
	{
		rotator = ParentPawn->GetBaseAimRotation();
	}	auto controller = ParentPawn->GetController();

	pos = GetSource();

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = controller;
	spawnParams.Instigator = ParentPawn;

	if (CanShoot() && Alive)
		if (AmmoType == 1) {
			SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, pos, rotator, spawnParams);
		}
		else {
			SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(GranadeClass, pos, rotator, spawnParams);
		}

	if (SpawnedProjectile)
	{
		ActionHappening = true;
		UE_LOG(LogTemp, Warning, TEXT("Successfully spawned projectile!"));

		// Define a delegate for the event
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("ActionStopped"));

		// Set a timer that calls ActionStopped using the delegate
		GetWorldTimerManager().SetTimer(timer, TimerDelegate, ResetTime, false);

		// Broadcast to the Delegate CallOnRifleAttack
		OnRifleAttack.Broadcast();
		UseAmmo();

	}
}

bool ABaseRifle::CanShoot() const {
	return !ActionHappening && Alive && CurrAmmo > 0;
}

FVector ABaseRifle::GetSource()
{
	return baseRifleMesh->GetSocketLocation(WeaponSocketName);
}

void ABaseRifle::UseAmmo() {
	CurrAmmo = FMath::Max(CurrAmmo - 1, 0);
	OnAmmoChanged.Broadcast(CurrAmmo, MaxAmmo, ReserveAmmo);
}

void ABaseRifle::ActionStopped() {
	ActionHappening = false;
	HandleActionFinished();
}

void ABaseRifle::RequestReload() {
	if (!ActionHappening) {
		ActionHappening = true;
		OnReloadStart.Broadcast();
	}
}

void ABaseRifle::ReloadAmmo() {
	float ammoNeeded = MaxAmmo - CurrAmmo; // calculates # of bullets to fill clip
	float ammoToLoad = FMath::Min(ammoNeeded, ReserveAmmo); // check that we can supply that amount

	// apply the reload
	CurrAmmo += ammoToLoad;
	ReserveAmmo -= ammoToLoad;

	OnAmmoChanged.Broadcast(CurrAmmo, MaxAmmo, ReserveAmmo);
}

void ABaseRifle::AddAmmo(float amount) {
	ReserveAmmo += amount;
	OnAmmoChanged.Broadcast(CurrAmmo, MaxAmmo, ReserveAmmo);
}

void ABaseRifle::OwnerDied()
{
	Alive = false;
	TurnFlashlightOff();
}

void ABaseRifle::HandleActionFinished()
{
	OnActionStopped.Broadcast();
}

// flashlight functions
void ABaseRifle::ToggleFlashlight()
{
	bFlashlightOn = !bFlashlightOn;
	FlashlightComponent->SetVisibility(bFlashlightOn);
}

void ABaseRifle::TurnFlashlightOn()
{
	if (!bFlashlightOn)
	{
		bFlashlightOn = true;
		FlashlightComponent->SetVisibility(true);
	}
}

void ABaseRifle::TurnFlashlightOff()
{
	if (bFlashlightOn)
	{
		bFlashlightOn = false;
		FlashlightComponent->SetVisibility(false);
	}
}
