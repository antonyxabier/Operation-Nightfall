// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_WanderLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"

UBTTask_WanderLocation::UBTTask_WanderLocation()
{
	NodeName = TEXT("Wander Location");
}

EBTNodeResult::Type UBTTask_WanderLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem)
	{
		return EBTNodeResult::Failed;
	}
	FNavLocation RandomLocation;
	const FVector Origin = ControlledPawn->GetActorLocation();
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, WanderRadius, RandomLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(DestinationKey.SelectedKeyName, RandomLocation.Location);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}