// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_HealthComponent.h"



// Sets default values for this component's properties
UAC_HealthComponent::UAC_HealthComponent() 
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	MaxHealth = 5.0f;
	CurrentHealth = MaxHealth;
	bIsDead= false;


	// ...
}


// Called when the game starts
void UAC_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	bIsDead = false;

	// Bind to the Take Any Damage event
	AActor* Owner = GetOwner();
	if (Owner) {
		Owner->OnTakeAnyDamage.AddDynamic(this, &UAC_HealthComponent::HandleDamage);

	}
}

void UAC_HealthComponent::HandleDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser) {

	if (Damage == 0.0f)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Damage received: %f"), Damage);
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);

	// Fire OnHurt event with the ratio
	if (Damage > 0.0f)
	{
		// Took damage
		OnHurt.Broadcast(CurrentHealth / MaxHealth);
		

		if (CurrentHealth <= 0.0f)
		{
			
			OnDeath.Broadcast(0.0f);
			
		}
	}
	else {
		//took healing
		OnHurt.Broadcast(CurrentHealth / MaxHealth);
	}
	
	// If health reaches 0, trigger death
	if (CurrentHealth <= 0.0f && !bIsDead)
	{
		bIsDead = true;
		OnDeath.Broadcast(0.0);
		
	}
}

// Called every frame
void UAC_HealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAC_HealthComponent::RestoreFullHealth() {
	CurrentHealth = MaxHealth;
	bIsDead = false;
	OnHurt.Broadcast(CurrentHealth / MaxHealth);
}