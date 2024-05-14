// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MultiplayerHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
	
	class UTexture2D* CrosshairsCenter;
	UTexture2D* CrosshairsLeft;
	UTexture2D* CrosshairsRight;
	UTexture2D* CrosshairsTop;
	UTexture2D* CrosshairsBottom;
	float CrosshairSpread;
	FLinearColor CrosshairColour;
};

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMP_API AMultiplayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "Player stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;
	
	class UCharacterOverlay* CharacterOverlay;

protected:
	virtual void BeginPlay() override;
	void AddCharacterOverlay();
	
private:
	FHUDPackage HUDPackage;

	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColour);

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;

public:
	FORCEINLINE void SetHudPackage(const FHUDPackage& Package) { HUDPackage = Package; }
	
};
