// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MultiCharAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERTEMP_API UMultiCharAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	// this is basically begin play
	virtual void NativeInitializeAnimation() override;

	// called every second, like the tick function
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	// can only use blueprintreadonly on pvt variables if using meta specifier
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class AMultiplayerCharacter* MultiplayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
};