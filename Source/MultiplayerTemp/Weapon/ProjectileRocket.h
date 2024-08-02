// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileRocket.generated.h"

UCLASS()
class MULTIPLAYERTEMP_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileRocket();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;


private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RocketMesh;
	
};
