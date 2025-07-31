// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimation.h"
#include "KismetAnimationLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"


void UCharacterAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Pawn = TryGetPawnOwner();

	if (Pawn) {
		//IsValid
		Velocity = Pawn->GetVelocity().Size();
		
		// new model does not support strafing !! do not use ukismet lirbrary !!
		Direction = UKismetAnimationLibrary::CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());
	}
	else {
		//IsNotValid
		PreviewWindowUpdate();
	}
}

UCharacterAnimation::UCharacterAnimation() : DebugFire(false), DebugHit(false), DebugDeath(false), CurrentDeathAsset(nullptr), Velocity(0), Direction(0), ActionSlotName("Action")
{
	DeathSounds.Init(nullptr, 6);
}

void UCharacterAnimation::FireAnimation_Implementation()
{
	PlaySlotAnimationAsDynamicMontage(FireAsset, ActionSlotName, .1, .1, 1, 1, 0);
	UGameplayStatics::PlaySound2D(this, FireSound);
}

void UCharacterAnimation::HitAnimation(float notUsed)
{
	if (HitAsset)
	{
		PlaySlotAnimationAsDynamicMontage(HitAsset, ActionSlotName, .1, .1, 1, 1, 0);
	}
}

void UCharacterAnimation::DeathAnimation()
{
	int32 RandomIndex = FMath::RandRange(0, DeathSounds.Num() - 1);
	UGameplayStatics::PlaySound2D(this, DeathSounds[RandomIndex]);
	if (DeathAssets.Num() > 0)
	{
		CurrentDeathAsset = DeathAssets[FMath::RandRange(0, DeathAssets.Num() - 1)];
		//PlaySlotAnimationAsDynamicMontage(CurrentDeathAsset, ActionSlotName);
		// Get animation duration
		const float AnimDuration = CurrentDeathAsset->GetPlayLength();
		FTimerHandle DeathEndTimerHandle;
		// Set timer to call DeathEnded after the animation duration
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(
				DeathEndTimerHandle,
				this,
				&UCharacterAnimation::DeathEnded,
				AnimDuration,
				false
			);
		}
	}
	}

void UCharacterAnimation::ReloadAnimation()
{
	UGameplayStatics::PlaySound2D(this, ReloadSound);
	if (ReloadAsset) {
		PlaySlotAnimationAsDynamicMontage(ReloadAsset, ActionSlotName, .1, .1, 1, 1, 0);

		// Delay the actual ammo reload to match animation length
		const float AnimDuration = ReloadAsset->GetPlayLength();
		if (UWorld* World = GetWorld())
		{
			FTimerHandle TempHandle;
			World->GetTimerManager().SetTimer(
				TempHandle,
				this,
				&UCharacterAnimation::ReloadEnded,
				AnimDuration,
				false
			);
		}
	}
}


void UCharacterAnimation::PreviewWindowUpdate_Implementation()
{
	if (DebugFire)
	{
		DebugFire = false;
		FireAnimation();
	}
	if (DebugHit)
	{
		DebugHit = false;
		HitAnimation();
	}
	if (DebugDeath)
	{
		DebugDeath = false;
		DeathAnimation();
	}
	if (DebugReload)
	{
		DebugReload = false;
		ReloadAnimation();
	}


}

void UCharacterAnimation::DeathEnded()
{
	OnDeathEnded.Broadcast();
}

void UCharacterAnimation::ReloadEnded()
{
	OnReloadNow.Broadcast(); // This will trigger ReloadAmmo in ABaseRifle
	OnActionEnded.Broadcast(); // Resets ActionHappening
}