// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMP_API AMultiplayerGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMultiplayerGameMode();
	virtual void Tick(float DeltaTime) override;
	virtual void PlayerEliminated(class AMultiplayerCharacter* EliminatedCharacter, class AMultiplayerPlayerController* VictimController, AMultiplayerPlayerController* AttackerController);
	virtual void RequestRespawn(class ACharacter* ElimmedCharacter, AController* ElimmedController);

	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 10.f;

	float LevelStartingTime = 0.f;

protected:
	virtual void BeginPlay() override;
	
private:
	float CountdownTime = 0.f;
};
