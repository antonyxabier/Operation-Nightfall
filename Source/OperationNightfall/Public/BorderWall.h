// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BorderWall.generated.h"

class UCombatZoneWarningWidget;
class UUserWidget;

UCLASS()
class OPERATIONNIGHTFALL_API ABorderWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABorderWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* WallTrigger;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, 
						class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, 
						class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OutOfBoundsDeath();

	FTimerHandle OutOfBoundsTimer;

	UPROPERTY(EditAnywhere, Category = "OutOfBounds")
	float OutOfBoundsTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> CombatZoneWidgetClass;

	UCombatZoneWarningWidget* CombatZoneWidgetInstance;

	FTimerHandle CountdownUpdateHandle;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateCountdown();

};
