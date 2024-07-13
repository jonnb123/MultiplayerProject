// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerTemp/Character/MultiplayerCharacter.h"
#include "MultiplayerTemp/PlayerController/MultiplayerPlayerController.h"
#include "MultiplayerTemp/PlayerState/MultiplayerPlayerState.h"

namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

AMultiplayerGameMode::AMultiplayerGameMode()
{
	bDelayedStart = true;
}

void AMultiplayerGameMode::BeginPlay()
{
	Super::BeginPlay();

	LevelStartingTime = GetWorld()->GetTimeSeconds();
}



void AMultiplayerGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState == MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			StartMatch();
		}
	}
	else if (MatchState == MatchState::InProgress)
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}

	else if (MatchState == MatchState::Cooldown)
	{
		CountdownTime = CooldownTime + WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartingTime;
		if (CountdownTime <= 0.f)
		{
			RestartGame();
		}
	}
}

void AMultiplayerGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for(FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AMultiplayerPlayerController* MutiplayerPlayer = Cast<AMultiplayerPlayerController>(*It);
		if (MutiplayerPlayer)
		{
			MutiplayerPlayer->OnMatchStateSet(MatchState);
		}
	}
}

void AMultiplayerGameMode::PlayerEliminated(class AMultiplayerCharacter* EliminatedCharacter, class AMultiplayerPlayerController* VictimController, AMultiplayerPlayerController* AttackerController)
{
	AMultiplayerPlayerState* AttackerPlayerState = AttackerController ? Cast<AMultiplayerPlayerState>(AttackerController->PlayerState) : nullptr;
	AMultiplayerPlayerState* VictimPlayerState = VictimController ? Cast<AMultiplayerPlayerState>(VictimController->PlayerState) : nullptr;

	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState)
	{
		AttackerPlayerState->AddToScore(1.f);
	}

	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}
	
	if (EliminatedCharacter)
	{
		EliminatedCharacter->Elim();
	}
}

void AMultiplayerGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}



	
