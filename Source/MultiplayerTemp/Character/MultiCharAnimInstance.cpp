// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerTemp/Character/MultiCharAnimInstance.h"
#include "MultiplayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

	bWeaponEquipped = MultiplayerCharacter->IsWeaponEquipped();
	EquippedWeapon = MultiplayerCharacter->GetEquippedWeapon();
	bIsCrouched = MultiplayerCharacter->bIsCrouched;

	bAiming = MultiplayerCharacter->IsAiming();

	// offset yaw for strafing
	FRotator AimRotation = MultiplayerCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(MultiplayerCharacter->GetVelocity());
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	// lower InterpSpeed to improve animation smoothness
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 4.f);
	YawOffset = DeltaRotation.Yaw;

	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = MultiplayerCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);

	AO_Yaw = MultiplayerCharacter->GetAO_Yaw();
	AO_Pitch = MultiplayerCharacter->GetAO_Pitch();

	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && MultiplayerCharacter->GetMesh())
	{
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), RTS_World);
		// these two variables are necessary for transformtobonespace below
		FVector OutPosition;
		FRotator OutRotation;
		// want the left hand to be relative to the character's right hand bone
		MultiplayerCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(),
			FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));
	}
	
}
