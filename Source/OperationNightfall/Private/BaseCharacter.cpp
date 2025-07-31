// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "BaseRifle.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter() : TeamID(0)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	GetMesh()->SetRelativeRotation(FRotator(0., -90.0, 0.));
	GetMesh()->SetRelativeLocation(FVector(0., 0., -90.));

	WeaponClass = ABaseRifle::StaticClass();
	// Create Child Actor Component
	ChildActorComponent = CreateDefaultSubobject<UChildActorComponent>("ChildActorComponent");
	ChildActorComponent->SetupAttachment(GetMesh(), "PlaceWeaponHere");

	HealthComponent = CreateDefaultSubobject<UAC_HealthComponent>(TEXT("HealthComponent"));

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (ChildActorComponent) {
		// Set the child actor class
		if (WeaponClass)
		{
			ChildActorComponent->SetChildActorClass(WeaponClass);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ChildActorComponent is not set in %s"), *GetName());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponClass is not set in %s"), *GetName());
	}

	WeaponObject = Cast<ABaseRifle>(ChildActorComponent->GetChildActor());
	CharacterAnimation = Cast<UCharacterAnimation>(GetMesh()->GetAnimInstance());

	if (WeaponObject && CharacterAnimation) {

		WeaponObject->OnRifleAttack.AddDynamic(CharacterAnimation, &UCharacterAnimation::FireAnimation);
		CharacterAnimation->OnReloadNow.AddDynamic(WeaponObject, &ABaseRifle::ReloadAmmo);
		CharacterAnimation->OnActionEnded.AddDynamic(WeaponObject, &ABaseRifle::ActionStopped);
		WeaponObject->OnReloadStart.AddDynamic(CharacterAnimation, &UCharacterAnimation::ReloadAnimation);
	}
	HealthComponent->OnHurt.AddDynamic(this, &ABaseCharacter::HandleHurt);
	HealthComponent->OnDeath.AddDynamic(this, &ABaseCharacter::HandleDeathStart);

	AController* contrlr = GetController();
	if (contrlr)
	{
		IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(contrlr);
		if (TeamAgent)
		{
			TeamAgent->SetGenericTeamId(TeamID);
		}
	}

}

void ABaseCharacter::HandleHurt(float ratio)
{
			
		
	CharacterAnimation->HitAnimation(ratio);

}

void ABaseCharacter::HandleDeathStart(float ratio)
{
	
	
	CharacterAnimation->DeathAnimation();
	this->SetActorEnableCollision(false);
	WeaponObject->OwnerDied();

}

void ABaseCharacter::OnReloadPressed()
{
	WeaponObject->RequestReload();
	//WeaponObject->ReloadAmmo();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
//void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

//}

