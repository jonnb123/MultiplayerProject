// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerPlayerController.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "MultiplayerTemp/Character/MultiplayerCharacter.h"
#include "MultiplayerTemp/HUD/MultiplayerHUD.h"
#include "MultiplayerTemp/HUD/CharacterOverlay.h"



void AMultiplayerPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CharacterHUD = Cast<AMultiplayerHUD>(GetHUD());
}

void AMultiplayerPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDTime();
	CheckTimeSync(DeltaTime);
	
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
		int32 Minutes = FMath::FloorToInt(CountdownTime/60.f);
		int32 Seconds = CountdownTime - Minutes * 60.f;
		
		
		FString CountdownText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		CharacterHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(CountdownText));
	}
}



void AMultiplayerPlayerController::SetHUDTime()
{
	uint32 SecondsLeft = FMath::CeilToInt(MatchTime - GetServerTime());
	if (CountdownInt != SecondsLeft)
	{
		SetHUDMatchCountdown(MatchTime - GetServerTime());
	}

	CountdownInt = SecondsLeft;
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
