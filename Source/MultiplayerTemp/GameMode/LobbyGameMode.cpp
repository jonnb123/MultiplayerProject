// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"

// this is called whenever a player joins the game
void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	int NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 2)
	{
		if (UWorld* World = GetWorld())
		{
			bUseSeamlessTravel = true;
			// the ?listen means it's open as a listen server for clients to connect to
			World->ServerTravel(FString("/Game/Maps/MultiplayerMap?listen"));
		}
	}
}
