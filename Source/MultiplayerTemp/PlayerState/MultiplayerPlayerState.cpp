// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerPlayerState.h"

#include "MultiplayerTemp/Character/MultiplayerCharacter.h"
#include "MultiplayerTemp/PlayerController/MultiplayerPlayerController.h"
#include "Net/UnrealNetwork.h"

void AMultiplayerPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// registers defeats for replication
	DOREPLIFETIME(AMultiplayerPlayerState, Defeats);
}


void AMultiplayerPlayerState::AddToScore(float ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);

	Character = Character == nullptr ? Cast<AMultiplayerCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AMultiplayerPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());	
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
			Controller->SetHUDScore(GetScore());	
		}
	}
	
}

void AMultiplayerPlayerState::AddToDefeats(int32 DefeatsAmount)
{
	Defeats += DefeatsAmount; // as soon as this line is hit, as defeats is replicated, so it goes down to clients

	Character = Character == nullptr ? Cast<AMultiplayerCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AMultiplayerPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}


void AMultiplayerPlayerState::OnRep_Defeats()
{
	Character = Character == nullptr ? Cast<AMultiplayerCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<AMultiplayerPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDDefeats(Defeats);
		}
	}
}


