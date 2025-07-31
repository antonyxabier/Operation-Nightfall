#include "BasePlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "BaseRifle.h"
#include <Kismet/KismetMathLibrary.h>
#include "AC_HealthComponent.h"
#include "CharacterAnimation.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

ABasePlayer::ABasePlayer()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComp->SetRelativeLocation(FVector(0., 90., 80.));
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->TargetArmLength = 120.f;
	SpringArmComp->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArmComp);

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));
	StimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
	StimuliSource->bAutoRegister = true;

	CurrentVisionMode = EVisionMode::None;

	NightVisionPostProcessMaterial = nullptr; 
	ThermalVisionPostProcessMaterial = nullptr; 
}

void ABasePlayer::BeginPlay() {
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("We Need A Player Controller to spawn the HUD"));
		Destroy();
	}

	if (!HealthComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("We Need A Health Component to spawn the HUD"));
		Destroy();
	}

	// Create and add HUD to viewport
	if (HUDClass)
	{
		HUDWidget = CreateWidget(PlayerController, HUDClass);
		UPlayerHUD* PlayerHUD = Cast<UPlayerHUD>(HUDWidget);
		HUDWidget->AddToViewport();

		if (PlayerHUD)
		{
			// bind health component and character animation
			HealthComponent->OnHurt.AddDynamic(PlayerHUD, &UPlayerHUD::SetHealth);
			HealthComponent->OnDeath.AddDynamic(PlayerHUD, &UPlayerHUD::SetHealth);
			// !!! missing binding for healing pickup to update UI !!!
			CharacterAnimation->OnDeathEnded.AddDynamic(this, &ABasePlayer::K2_DestroyActor);
			
			// bind weapon object
			WeaponObject->OnAmmoChanged.AddDynamic(PlayerHUD, &UPlayerHUD::SetAmmo);
			WeaponObject->ReloadAmmo();
		
		}
	}
}

bool ABasePlayer::CanPickup()
{
	return true;
}

void ABasePlayer::PickupAmmo(float amount)
{
	WeaponObject->AddAmmo(amount);
}

void ABasePlayer::HandleHurt(float ratio)
{
	Super::HandleHurt(ratio);
	UpdateHUD(ratio);
}

void ABasePlayer::HandleDeathStart(float ratio)
{
	Super::HandleDeathStart(ratio);
	OnPlayerLost.Broadcast();
	UpdateHUD(ratio);
	DisableInput(PlayerController);
}

void ABasePlayer::UpdateHUD(float HealthRatio) {
	UPlayerHUD* PlayerHUD = Cast<UPlayerHUD>(HUDWidget);
	PlayerHUD->SetHealth(HealthRatio);
}

void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("TurnRight", this, &ABasePlayer::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ABasePlayer::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABasePlayer::InputAxisMoveForward);
	PlayerInputComponent->BindAxis("Strafe", this, &ABasePlayer::InputAxisStrafe);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABasePlayer::InputActionAttack);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABasePlayer::OnReloadPressed);
	//PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABasePlayer::InputActionReload);
	
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &ABasePlayer::InputActionPause);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABasePlayer::InputActionToggleCrouch);
	
	PlayerInputComponent->BindAction("ToggleFlashlight", IE_Pressed, this, &ABasePlayer::InputActionToggleFlashlight);
	PlayerInputComponent->BindAction("ToggleNightVision", IE_Pressed, this, &ABasePlayer::InputActionToggleNightVision);
	PlayerInputComponent->BindAction("ToggleThermalVision", IE_Pressed, this, &ABasePlayer::InputActionToggleThermalVision);
}

void ABasePlayer::InputAxisMoveForward(float AxisValue)
{
	// Move the actor forward
	AddMovementInput(FRotator(0, GetControlRotation().Yaw, 0).Vector(), AxisValue);
}

void ABasePlayer::InputAxisStrafe(float AxisValue)
{
	FRotator Break = GetControlRotation();
	FRotator Make(0., Break.Yaw + 90, 0.);
	FVector WorldDirection = Make.Vector();

	// Strafe the actor 
	AddMovementInput(WorldDirection, AxisValue);

}

void ABasePlayer::InputActionAttack() {
	WeaponObject->Attack();
}

void ABasePlayer::InputActionReload() {
	WeaponObject->ReloadAmmo();
}

void ABasePlayer::InputActionPause() {
	if (!PauseMenuClass) {
		UE_LOG(LogTemp, Error, TEXT("Missing pause menu asset!"));
		return;
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	
	bool bIsPaused = GetWorld()->IsPaused();
	PC->SetPause(!bIsPaused);

	if (!PauseMenuWidget) {
		PauseMenuWidget = CreateWidget<UUserWidget>(PC, PauseMenuClass);
	}
	PauseMenuWidget->AddToViewport();
	FInputModeUIOnly UIInput;
	UIInput.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
	PC->SetInputMode(UIInput);
	PC->bShowMouseCursor = true;
}

void ABasePlayer::InputActionToggleFlashlight()
{
	if (WeaponObject) {
		WeaponObject->ToggleFlashlight();
	}
}

void ABasePlayer::InputActionToggleCrouch() {
	if (bIsCrouched) {
		CharacterAnimation->bIsCrouching = false;
		UnCrouch();
	}
	else {
		CharacterAnimation->bIsCrouching = true;
		Crouch();
	}
}

void ABasePlayer::PlayerWin() {
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->StopMovementImmediately();
		MovementComp->DisableMovement(); // Ensure no further movement attempts
		UE_LOG(LogTemp, Log, TEXT("ABasePlayer::PlayerWin - Player movement stopped."));
	}
	if (PlayerController) // Check if PlayerController is still valid
	{
		// Switch to UI-only input
		FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
	HUDWidget->RemoveFromParent();
	HUDWidget = nullptr;
}

void ABasePlayer::PlayerLost() {
	OnPlayerLost.Broadcast();

	// Always null-check pointers before dereferencing them!
	if (PlayerController) // Check if PlayerController is still valid
	{
		PlayerController->bShowMouseCursor = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ABasePlayer::PlayerLost - PlayerController is null when trying to show mouse cursor!"));
	}

	if (HUDWidget) // Check if HUDWidget is still valid
	{
		HUDWidget->RemoveFromParent();
		// It's safe to nullify AFTER you've used it and removed it from the hierarchy
		HUDWidget = nullptr;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ABasePlayer::PlayerLost - HUDWidget is null when trying to remove from parent!"));
	}
}

void ABasePlayer::InputActionToggleNightVision()
{
	if (CurrentVisionMode == EVisionMode::NightVision)
	{
		SetVisionMode(EVisionMode::None); // Turn off Night Vision if it's currently active
	}
	else
	{
		SetVisionMode(EVisionMode::NightVision); // Turn on Night Vision
	}
}

void ABasePlayer::InputActionToggleThermalVision()
{
	if (CurrentVisionMode == EVisionMode::ThermalVision)
	{
		SetVisionMode(EVisionMode::None); // Turn off Thermal Vision if it's currently active
	}
	else
	{
		SetVisionMode(EVisionMode::ThermalVision); // Turn on Thermal Vision
	}
}

void ABasePlayer::SetVisionMode(EVisionMode NewMode)
{
	// Log the intended mode change and current state for debugging
	UE_LOG(LogTemp, Log, TEXT("SetVisionMode called: NewMode = %s, CurrentVisionMode = %s"),
		*UEnum::GetValueAsString(NewMode), *UEnum::GetValueAsString(CurrentVisionMode));

	// If trying to set the same mode we are already in, and it's not None
	if (NewMode == CurrentVisionMode && NewMode != EVisionMode::None)
	{
		UE_LOG(LogTemp, Log, TEXT("Already in %s. No change needed (or handled by toggle)."), *UEnum::GetValueAsString(NewMode));
		return;
	}

	// --- FIX: Explicitly remove currently active post-process materials ---
	if (Camera)
	{
		if (CurrentVisionMode == EVisionMode::NightVision && NightVisionPostProcessMaterial)
		{
			Camera->RemoveBlendable(NightVisionPostProcessMaterial);
			UE_LOG(LogTemp, Log, TEXT("Removed Night Vision blendable."));
		}
		else if (CurrentVisionMode == EVisionMode::ThermalVision && ThermalVisionPostProcessMaterial)
		{
			Camera->RemoveBlendable(ThermalVisionPostProcessMaterial);
			UE_LOG(LogTemp, Log, TEXT("Removed Thermal Vision blendable."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Camera is NULL. Cannot remove post-process blendables."));
	}

	CurrentVisionMode = NewMode; // Update the current mode

	switch (CurrentVisionMode)
	{
	case EVisionMode::None:
		UE_LOG(LogTemp, Log, TEXT("Vision Mode: Normal (OFF)."));
		// No post-process material needed for normal vision, already removed above
		break;

	case EVisionMode::NightVision:
		if (NightVisionPostProcessMaterial && Camera)
		{
			Camera->AddOrUpdateBlendable(NightVisionPostProcessMaterial, 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Vision Mode: Night Vision ON."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("NightVisionPostProcessMaterial is NULL or Camera is NULL. Cannot activate Night Vision. Forcing to Normal."));
			// If material is missing, immediately revert to None to prevent a stuck state
			CurrentVisionMode = EVisionMode::None;
		}
		break;

	case EVisionMode::ThermalVision:
		if (ThermalVisionPostProcessMaterial && Camera)
		{
			Camera->AddOrUpdateBlendable(ThermalVisionPostProcessMaterial, 1.0f);
			UE_LOG(LogTemp, Log, TEXT("Vision Mode: Thermal Vision ON."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ThermalVisionPostProcessMaterial is NULL or Camera is NULL. Cannot activate Thermal Vision. Forcing to Normal."));
			// If material is missing, immediately revert to None to prevent a stuck state
			CurrentVisionMode = EVisionMode::None;
		}
		break;
	}
}