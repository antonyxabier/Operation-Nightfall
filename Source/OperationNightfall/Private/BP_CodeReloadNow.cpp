// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_CodeReloadNow.h"
#include "BaseCharacter.h"

void UBP_CodeReloadNow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
}

void UBP_CodeReloadNow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (UCharacterAnimation* Anim = Cast<UCharacterAnimation>(MeshComp->GetAnimInstance())) {
		Anim->OnReloadNow.Broadcast();
	}
}

