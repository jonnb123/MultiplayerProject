// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MultiplayerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMP_API AMultiplayerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void SetHUDHealth(float Health, float MaxHealth);
	void OnPossess(APawn* InPawn) override;
protected:
	virtual void BeginPlay() override;
private:
	class AMultiplayerHUD* CharacterHUD;
};
