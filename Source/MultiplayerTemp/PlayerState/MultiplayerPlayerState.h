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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// rep notifies
	virtual  void OnRep_Score() override;

	UFUNCTION()
	virtual void OnRep_Defeats();
	
	

	
	void AddToScore(float ScoreAmount);
	void AddToDefeats(int32 DefeatsAmount);

private:
	UPROPERTY()
	class AMultiplayerCharacter* Character;
	UPROPERTY()
	class AMultiplayerPlayerController* Controller;

	UPROPERTY(ReplicatedUsing = OnRep_Defeats)	
	int32 Defeats;

	
};
