// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Components/ProgressBar.h"
#include "../OperationNightfall.h"
#include <Kismet/GameplayStatics.h>
#include <Blueprint/SlateBlueprintLibrary.h>


void UPlayerHUD::SetHealth(float p)
{
	if (HealthBar) {
		HealthBar->SetPercent(p);
	}
}

void UPlayerHUD::SetAmmo(float Curr, float Max, float Reserve) {
    CurrAmmo->SetText(FText::AsNumber(Curr));
    MaxAmmo->SetText(FText::AsNumber(Max));
    ReserveAmmo->SetText(FText::AsNumber(Reserve));
}

FVector UPlayerHUD::GetDestination()
{
	return EndPoint;
}

void UPlayerHUD::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	UpdateCrosshair();
}

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

    if (Crosshair)
    {
        // Initialize Dynamic Material
        DynamicMaterial = Crosshair->GetDynamicMaterial();

        if (DynamicMaterial)
        {
            
            //DynamicMaterial->SetVectorParameterValue(ColorName, SafeColor);
        }
        else
        {
            
        }
    }
    else
    { 
    }

}

void UPlayerHUD::UpdateCrosshair()
{
    APlayerController* PlayerController = GetOwningPlayer();
    if (!PlayerController) return;

    FVector2D ScreenPosition;
    FVector WorldPosition, WorldDirection;

    // Get crosshair position in screen space
    if (Crosshair)
    {
        ScreenPosition = GetCrosshairScreenPosition();
    }

    // Convert screen position to world space
    if (PlayerController->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldPosition, WorldDirection))
    {
        // Calculate End Point Once
        EndPoint = WorldPosition + (WorldDirection * 100000.0f);

        // Collision Detection
        FCollisionObjectQueryParams ObjectTypes;
        ObjectTypes.AddObjectTypesToQuery(ECC_Pawn);
        ObjectTypes.AddObjectTypesToQuery(ECC_WorldDynamic);

        FHitResult HitResult;
        FCollisionQueryParams TraceParams;
        TraceParams.AddIgnoredActor(PlayerController->GetPawn());

        bool bHit = GetWorld()->LineTraceSingleByObjectType(
            HitResult,
            WorldPosition,
            EndPoint,
            ObjectTypes,
            TraceParams
        );

        if (bHit)
        {
            ECollisionChannel Channel = HitResult.GetComponent()->GetCollisionObjectType();
            EndPoint = HitResult.ImpactPoint;
            if (Channel == ECC_Pawn)
                DynamicMaterial->SetVectorParameterValue(ColorName, DangerColor);
            else
                DynamicMaterial->SetVectorParameterValue(ColorName, SafeColor);
        }
        else
            DynamicMaterial->SetVectorParameterValue(ColorName, SafeColor);
    }
}

FVector2D UPlayerHUD::GetCrosshairScreenPosition()
{
    if (!Crosshair)
    {
        return FVector2D(0, 0);
    }

    // Get Cached Geometry of the Crosshair
    FGeometry CrosshairGeometry = Crosshair->GetCachedGeometry();
    FVector2D ViewportPosition;
    FVector2D PixelPosition;

    USlateBlueprintLibrary::LocalToViewport(GetWorld(), CrosshairGeometry, FVector2D(0, 0), PixelPosition, ViewportPosition);
    FVector2D CrosshairSize = USlateBlueprintLibrary::GetAbsoluteSize(CrosshairGeometry);

    // Return Center Position of Crosshair in Screen Space
    return PixelPosition + (CrosshairSize * 0.5f);
}