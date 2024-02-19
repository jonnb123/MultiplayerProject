// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerTemp/Character/MultiCharAnimInstance.h"
#include "MultiplayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMultiCharAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MultiplayerCharacter = Cast<AMultiplayerCharacter>(TryGetPawnOwner());
}

void UMultiCharAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MultiplayerCharacter == nullptr)
	{
		MultiplayerCharacter = Cast<AMultiplayerCharacter>(TryGetPawnOwner());
	}
	if (MultiplayerCharacter == nullptr) return;

	FVector Velocity = MultiplayerCharacter->GetVelocity();
	Velocity.Z = 0.f;
	// this is the magnitude of the vector
	Speed = Velocity.Size();

	bIsInAir = MultiplayerCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = MultiplayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
}
