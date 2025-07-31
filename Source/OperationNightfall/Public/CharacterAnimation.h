// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Sound/SoundBase.h"
#include "CharacterAnimation.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnimDeleg);

/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API UCharacterAnimation : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds);

	UCharacterAnimation();

	FAnimDeleg OnReloadNow;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Animation")
	void FireAnimation();
	virtual void FireAnimation_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void HitAnimation(float notused = 0.0f);

	UFUNCTION( BlueprintCallable, Category = "Animation")
	void DeathAnimation();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void ReloadAnimation();

	UPROPERTY(BlueprintAssignable)
	FAnimDeleg OnActionEnded;

	UPROPERTY(BlueprintAssignable)
	FAnimDeleg OnDeathEnded;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	bool DebugFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	bool DebugHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	bool DebugDeath;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
	bool DebugReload;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	bool bIsCrouching;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	float Velocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	float Direction;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	UAnimSequenceBase* FireAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	UAnimSequenceBase* HitAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	UAnimSequenceBase* ReloadAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	UAnimSequenceBase* DeathAsset;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Assets)
	UAnimSequenceBase* CurrentDeathAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Assets)
	TArray<UAnimSequenceBase*> DeathAssets;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	FName ActionSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	TObjectPtr<USoundBase> FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	TObjectPtr<USoundBase> ReloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	TArray<USoundBase*> DeathSounds;



	UFUNCTION(BlueprintNativeEvent)
	void PreviewWindowUpdate();
	virtual void PreviewWindowUpdate_Implementation();

private:
	UFUNCTION()
	void DeathEnded();

	UFUNCTION()
	void ReloadEnded();

	
};
