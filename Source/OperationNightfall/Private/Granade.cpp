// Fill out your copyright notice in the Description page of Project Settings.


#include "Granade.h"


AGranade::AGranade() 
{
	
	DamageAmount = 10.0;
}

void AGranade::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::HandleOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}