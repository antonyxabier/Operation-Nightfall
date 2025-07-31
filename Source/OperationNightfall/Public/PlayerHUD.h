// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	// Set Health
	UFUNCTION(BlueprintCallable)
	void SetHealth(float p);

    UFUNCTION(BlueprintCallable)
    void SetAmmo(float Curr, float Max, float Reserve);

	// Pure Function to Get Destination
	UFUNCTION(BlueprintPure)
	FVector GetDestination();

	// Override Tick for dynamic updates
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

protected:

    // Bindable Widgets
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UProgressBar* HealthBar;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD, meta = (BindWidget))
    UImage* Crosshair;

    // ammo widgets
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* CurrAmmo; // player current ammo
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* MaxAmmo; // max ammo gun can hold at a time
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* ReserveAmmo; // player's remaining ammo

    // Linear Colors
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    UMaterialInstanceDynamic* DynamicMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FName ColorName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FLinearColor SafeColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FLinearColor DangerColor;

    virtual void NativeConstruct() override;
    void UpdateCrosshair();

private:

    FVector EndPoint;

    // Get Screen Position of Crosshair
    UFUNCTION(BlueprintCallable, Category = "HUD")
    FVector2D GetCrosshairScreenPosition();
};