// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PlayerPickupInterface.h"
#include "CodeBasePickup.generated.h"

UCLASS()
class OPERATIONNIGHTFALL_API ACodeBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACodeBasePickup();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	UBoxComponent* CollisionBox;
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void DoPickup(TScriptInterface<IPlayerPickupInterface> PickupInterface);
	UFUNCTION()
	virtual void PostPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
