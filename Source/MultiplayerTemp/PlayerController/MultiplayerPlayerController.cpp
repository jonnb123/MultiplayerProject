// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerPlayerController.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerTemp/Character/MultiplayerCharacter.h"
#include "MultiplayerTemp/CombatComponents/CombatComponent.h"
#include "MultiplayerTemp/GameMode/MultiplayerGameMode.h"
#include "MultiplayerTemp/GameState/MultiplayerGameState.h"
#include "MultiplayerTemp/HUD/Announcement.h"
#include "MultiplayerTemp/HUD/MultiplayerHUD.h"
#include "MultiplayerTemp/HUD/CharacterOverlay.h"
#include "MultiplayerTemp/PlayerState/MultiplayerPlayerState.h"
#include "Net/UnrealNetwork.h"


void AMultiplayerPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterHUD = Cast<AMultiplayerHUD>(GetHUD());

	ServerCheckMatchState();

	
}

void AMultiplayerPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDTime();
	CheckTimeSync(DeltaTime);
	PollInit();
	
}

void AMultiplayerPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMultiplayerPlayerController, MatchState);
}

void AMultiplayerPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}



void AMultiplayerPlayerController::ServerCheckMatchState_Implementation()
{
	AMultiplayerGameMode* GameMode = Cast<AMultiplayerGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		WarmupTime = GameMode->WarmupTime;
		MatchTime = GameMode->MatchTime;
		CooldownTime = GameMode->CooldownTime;
		LevelStartingTime = GameMode->LevelStartingTime;
		MatchState = GameMode->GetMatchState();
		ClientJoinMidgame(MatchState, WarmupTime, MatchTime, CooldownTime, LevelStartingTime);
	}
}

void AMultiplayerPlayerController::ClientJoinMidgame_Implementation(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime)
{
	WarmupTime = Warmup;
	MatchTime = Match;
	CooldownTime = Cooldown;
	LevelStartingTime = StartingTime;
	MatchState = StateOfMatch;
	OnMatchStateSet(MatchState);
	if (CharacterHUD && MatchState == MatchState::WaitingToStart)
	{
		CharacterHUD->AddAnnouncement();
	}
}

void AMultiplayerPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AMultiplayerCharacter* MultiplayerCharacter = Cast<AMultiplayerCharacter>(InPawn))
	{
		SetHUDHealth(MultiplayerCharacter->GetHealth(), MultiplayerCharacter->GetMaxHealth());
	}
	
}



void AMultiplayerPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	CharacterHUD = CharacterHUD == nullptr ? Cast<AMultiplayerHUD>(GetHUD()) : CharacterHUD;
	bool bHudValid = CharacterHUD &&
		CharacterHUD->CharacterOverlay &&
			CharacterHUD->CharacterOverlay->HealthBar &&
				CharacterHUD->CharacterOverlay->HealthText;
	
	if (bHudValid)
	{
		const float HealthPercent = Health / MaxHealth;
		CharacterHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		CharacterHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
	else
	{
		bInitializeCharacterOverlay = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
	}
}

void AMultiplayerPlayerController::SetHUDScore(float Score)
{
	CharacterHUD = CharacterHUD == nullptr ? Cast<AMultiplayerHUD>(GetHUD()) : CharacterHUD;
	bool bHudValid = CharacterHUD &&
		CharacterHUD->CharacterOverlay &&
			CharacterHUD->CharacterOverlay->ScoreAmount;

	if (bHudValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		CharacterHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
	}
	else
	{
		bInitializeCharacterOverlay = true;
		HUDScore = Score;
	}
}

void AMultiplayerPlayerController::SetHUDDefeats(int32 Defeats)
{
	CharacterHUD = CharacterHUD == nullptr ? Cast<AMultiplayerHUD>(GetHUD()) : CharacterHUD;
	bool bHudValid = CharacterHUD &&
		CharacterHUD->CharacterOverlay &&
			CharacterHUD->CharacterOverlay->DefeatsAmount;

	if (bHudValid)
	{
		FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
		CharacterHUD->CharacterOverlay->DefeatsAmount->SetText(FText::FromString(DefeatsText));
	}
	else
	{
		bInitializeCharacterOverlay = true;
		HUDDefeats = Defeats;
	}
}

void AMultiplayerPlayerController::SetHUDWeaponAmmo(int32 Ammo)
{
	CharacterHUD = CharacterHUD == nullptr ? Cast<AMultiplayerHUD>(GetHUD()) : CharacterHUD;
	bool bHudValid = CharacterHUD &&
		CharacterHUD->CharacterOverlay &&
			CharacterHUD->CharacterOverlay->WeaponAmmoAmount;

	if (bHudValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		CharacterHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(AmmoText));
	}
}

void AMultiplayerPlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
	CharacterHUD = CharacterHUD == nullptr ? Cast<AMultiplayerHUD>(GetHUD()) : CharacterHUD;
	bool bHudValid = CharacterHUD &&
		CharacterHUD->CharacterOverlay &&
			CharacterHUD->CharacterOverlay->CarriedAmmoAmount;

	if (bHudValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		CharacterHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
	}
}

void AMultiplayerPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	CharacterHUD = CharacterHUD == nullptr ? Cast<AMultiplayerHUD>(GetHUD()) : CharacterHUD;
	bool bHudValid = CharacterHUD &&
		CharacterHUD->CharacterOverlay &&
			CharacterHUD->CharacterOverlay->MatchCountdownText;

	if (bHudValid)
	{
		if (CountdownTime < 0.f)
		{
			CharacterHUD->CharacterOverlay->MatchCountdownText->SetText(FText());
			return;
		}
		int32 Minutes = FMath::FloorToInt(CountdownTime/60.f);
		int32 Seconds = CountdownTime - Minutes * 60.f;
		
		
		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		CharacterHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(CountdownText));
	}
}

void AMultiplayerPlayerController::SetHUDAnnouncementCountdown(float CountdownTime)
{
	CharacterHUD = CharacterHUD == nullptr ? Cast<AMultiplayerHUD>(GetHUD()) : CharacterHUD;
	bool bHudValid = CharacterHUD &&
		CharacterHUD->Announcement &&
			CharacterHUD->Announcement->WarmupTime;

	if (bHudValid)
	{
		if (CountdownTime < 0.f)
		{
			CharacterHUD->Announcement->WarmupTime->SetText(FText());
			return;
		}
		
		int32 Minutes = FMath::FloorToInt(CountdownTime/60.f);
		int32 Seconds = CountdownTime - Minutes * 60.f;
		
		
		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		CharacterHUD->Announcement->WarmupTime->SetText(FText::FromString(CountdownText));
	}
}


void AMultiplayerPlayerController::SetHUDTime()
{
	float TimeLeft = 0.f;
	if (MatchState == MatchState::WaitingToStart) TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::InProgress) TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	else if (MatchState == MatchState::Cooldown) TimeLeft = CooldownTime + WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	
	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);

	if (HasAuthority())
	{
		MultiplayerGameMode = MultiplayerGameMode == nullptr ? Cast<AMultiplayerGameMode>(UGameplayStatics::GetGameMode(this)) : MultiplayerGameMode;
		if (MultiplayerGameMode)
		{
			SecondsLeft = FMath::CeilToInt(MultiplayerGameMode->GetCountdownTime() + LevelStartingTime);
		}
	}
	
	if (CountdownInt != SecondsLeft)
	{
		if(MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown)
		{
			SetHUDAnnouncementCountdown(TimeLeft);
		}
		if (MatchState == MatchState::InProgress)
		{
			SetHUDMatchCountdown(TimeLeft);
		}
	}

	CountdownInt = SecondsLeft;
}

void AMultiplayerPlayerController::PollInit()
{
	if (CharacterOverlay == nullptr)
	{
		if (CharacterHUD && CharacterHUD->CharacterOverlay)
		{
			CharacterOverlay = CharacterHUD->CharacterOverlay;
			if (CharacterOverlay)
			{
				SetHUDHealth(HUDHealth, HUDMaxHealth);
				SetHUDScore(HUDScore);
				SetHUDDefeats(HUDDefeats);
			}
		}
	}
}


void AMultiplayerPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequest, ServerTimeOfReceipt);
}

void AMultiplayerPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest,
	float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5f * RoundTripTime);

	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}


float AMultiplayerPlayerController::GetServerTime()
{
	if (HasAuthority()) return GetWorld()->GetTimeSeconds();
	return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void AMultiplayerPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void AMultiplayerPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;

	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}



void AMultiplayerPlayerController::OnRep_MatchState()
{
	if (MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if (MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}


void AMultiplayerPlayerController::HandleMatchHasStarted()
{
	CharacterHUD = CharacterHUD == nullptr ? Cast<AMultiplayerHUD>(GetHUD()) : CharacterHUD;
	if (CharacterHUD)
	{
		CharacterHUD->AddCharacterOverlay();
		if (CharacterHUD->Announcement)
		{
			CharacterHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMultiplayerPlayerController::HandleCooldown()
{
	CharacterHUD = CharacterHUD == nullptr ? Cast<AMultiplayerHUD>(GetHUD()) : CharacterHUD;
	if (CharacterHUD)
	{
		CharacterHUD->CharacterOverlay->RemoveFromParent();
		bool bHUDValid = CharacterHUD->Announcement &&
			CharacterHUD->Announcement->AnnouncementText &&
			CharacterHUD->Announcement->InfoText;
		if (bHUDValid)
		{
			CharacterHUD->Announcement->SetVisibility(ESlateVisibility::Visible);
			FString AnnouncementText("New Match Starts In: ");
			CharacterHUD->Announcement->AnnouncementText->SetText(FText::FromString(AnnouncementText));

			AMultiplayerGameState* MultiplayerGameState = Cast<AMultiplayerGameState>(UGameplayStatics::GetGameState(this));
			AMultiplayerPlayerState* MultiplayerPlayerState = GetPlayerState<AMultiplayerPlayerState>();
			if (MultiplayerGameState && MultiplayerPlayerState)
			{
				TArray<AMultiplayerPlayerState*> TopPlayers = MultiplayerGameState->TopScoringPlayers;
				FString InfoTextString;
				if (TopPlayers.Num() == 0)
				{
					InfoTextString = FString("There is no winner.");
				}
				else if (TopPlayers.Num() == 1 && TopPlayers[0] == MultiplayerPlayerState)
				{
					InfoTextString = FString("You are the winner!");
				}
				else if (TopPlayers.Num() == 1)
				{
					InfoTextString = FString::Printf(TEXT("Winner: \n%s"), *TopPlayers[0]->GetPlayerName());
				}
				else if (TopPlayers.Num() > 1)
				{
					InfoTextString = FString("Players tied for the win:\n");
					for (auto TiedPlayer : TopPlayers)
					{
						InfoTextString.Append(FString::Printf(TEXT("%s\n"),*TiedPlayer->GetPlayerName()));
					}
				}
				
				CharacterHUD->Announcement->InfoText->SetText(FText::FromString(InfoTextString));
			}
		}
	}
	AMultiplayerCharacter* MultiplayerCharacter = Cast<AMultiplayerCharacter>(GetPawn());
	if  (MultiplayerCharacter && MultiplayerCharacter->GetCombat())
	{
		MultiplayerCharacter->bDisableGameplay = true;
		MultiplayerCharacter->GetCombat()->FireButtonPressed(false);
	}
}

