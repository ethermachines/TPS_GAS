// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HitScanWeapon.generated.h"

class UParticleSystem;

UCLASS()
class TPS_GAS_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()

private:
	virtual void Fire(const FVector& HitTarget) override;

	UPROPERTY(EditAnywhere)
	float Damage = 20.0f;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticles;
	
};
