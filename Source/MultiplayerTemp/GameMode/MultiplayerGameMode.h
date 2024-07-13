// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerGameMode.generated.h"

namespace MatchState
{
	// Match duration has been reached. Display winner and begin cooldown timer
	extern MULTIPLAYERTEMP_API const FName Cooldown; 
}

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

	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;

	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 10.f;
	
	float LevelStartingTime = 0.f;

protected:
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;
	
	
private:
	float CountdownTime = 0.f;
};
