// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MultiplayerPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMP_API AMultiplayerPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	virtual  void OnRep_Score() override;
	void AddToScore(float ScoreAmount);

private:

	class AMultiplayerCharacter* Character;
	class AMultiplayerPlayerController* Controller;
	
	
	
};
