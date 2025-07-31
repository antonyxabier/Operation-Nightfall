#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CodePauseMenuUI.h"
#include "ControlsDisplay.h"
#include "PlayerHUD.h"
#include "PlayerPickupInterface.h"
#include "Materials/MaterialInterface.h"
#include "BasePlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerLost);

UENUM(BlueprintType)
enum class EVisionMode : uint8
{
	None UMETA(DisplayName = "Normal Vision"),
	NightVision UMETA(DisplayName = "Night Vision"),
	ThermalVision UMETA(DisplayName = "Thermal Vision")
};

UCLASS()
class OPERATIONNIGHTFALL_API ABasePlayer : public ABaseCharacter, public IPlayerPickupInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Character)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = Character)
	class UCameraComponent* Camera;

	// pause menu reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<UUserWidget> PauseMenuClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
	UUserWidget* PauseMenuWidget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
	APlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UAIPerceptionStimuliSourceComponent* StimuliSource;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UPlayerHUD> HUDClass;

	void HandleHurt(float ratio) override;

	void HandleDeathStart(float ratio) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vision Modes")
	EVisionMode CurrentVisionMode;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Vision Modes")
	UMaterialInterface* NightVisionPostProcessMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Vision Modes")
	UMaterialInterface* ThermalVisionPostProcessMaterial;

	// Timer handle for battery drain
	FTimerHandle BatteryDrainTimerHandle;

public:
	ABasePlayer();

	UUserWidget* GetHUDWidget() { return HUDWidget; }
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// lose state delegate
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnPlayerLost OnPlayerLost;

	void PlayerWin(); // win state

	bool CanPickup()  override;
	void PickupAmmo(float amount); // handles ammo pickup

	// cheats variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cheats")
	bool bGodMode = false;

	void UpdateHUD(float HealthRatio);

	UFUNCTION(BlueprintCallable, Category = "Vision Modes")
	void InputActionToggleNightVision(); // Bound to input

	UFUNCTION(BlueprintCallable, Category = "Vision Modes")
	void InputActionToggleThermalVision();

	UFUNCTION(BlueprintCallable, Category = "Vision Modes")
	void SetVisionMode(EVisionMode NewMode); // Internal function to switch modes

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }



private:
	// movement input
	void InputAxisMoveForward(float AxisValue);
	void InputAxisStrafe(float AxisValue);

	void InputActionPause();
	void InputActionToggleFlashlight();

	// crouching input
	void InputActionToggleCrouch();

	// rifle input
	void InputActionAttack();
	void InputActionReload();

	UPROPERTY()
	UUserWidget* HUDWidget;

	void PlayerLost(); // lose state
};
