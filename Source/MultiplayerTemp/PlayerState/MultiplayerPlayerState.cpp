// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerPlayerState.h"

#include "MultiplayerTemp/Character/MultiplayerCharacter.h"
#include "MultiplayerTemp/PlayerController/MultiplayerPlayerController.h"




void AMultiplayerPlayerState::AddToScore(float ScoreAmount)
{
	Score += ScoreAmount;

	Character = Character == nullptr ? Cast<AMultiplayerCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AMultiplayerPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(Score);	
		}
	}
}

void AMultiplayerPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	Character = Character == nullptr ? Cast<AMultiplayerCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AMultiplayerPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(Score);	
		}
	}
	
}


