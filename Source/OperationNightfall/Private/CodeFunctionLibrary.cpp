// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UCodeFunctionLibrary::ShowMouse(UObject* WorldContextObject)
{
    if (!WorldContextObject) return;

    APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
    if (PC)
    {
        PC->bShowMouseCursor = true;

        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);
        PC->SetInputMode(InputMode);
    }
}

void UCodeFunctionLibrary::HideMouse(UObject* WorldContextObject)
{
    if (!WorldContextObject) return;

    APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
    if (IsValid(PC))
    {
        PC->bShowMouseCursor = false;

        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
    }
}
