// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KillPlane.generated.h"

UCLASS()
class OPERATIONNIGHTFALL_API AKillPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AKillPlane();

protected:
	
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* KillBox;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, 
						class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
