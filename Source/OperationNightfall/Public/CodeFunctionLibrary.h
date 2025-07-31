// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CodeFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONNIGHTFALL_API UCodeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    static void ShowMouse(UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "UI")
    static void HideMouse(UObject* WorldContextObject);
};
