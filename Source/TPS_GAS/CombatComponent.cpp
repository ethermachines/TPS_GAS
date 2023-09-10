// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "TPS_GASCharacter.h"
#include "Weapon.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Player)
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;
	}

}

void UCombatComponent::StartFireTimer()
{
	/*if (EquippedWeapon == nullptr || Player == nullptr) return;
	Player->GetWorldTimerManager().SetTimer(
		FireTimer,
		this,
		&UCombatComponent::FireTimerFinished,
		EquippedWeapon->FireDelay
	);*/
}

void UCombatComponent::FireTimerFinished()
{
}

bool UCombatComponent::CanFire()
{
	return false;
}


void UCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if (Player && bFireButtonPressed)
	{
		//Player->PlayFireMontage(bAiming);
	}
}

void UCombatComponent::Fire()
{
	if (CanFire())
	{
		bCanFire = false;
		if (EquippedWeapon)
		{
			//CrosshairShootingFactor = .75f;

			FireHitScanWeapon();
			/*switch (EquippedWeapon->FireType)
			{
			case EFireType::EFT_Projectile:
				FireProjectileWeapon();
				break;
			case EFireType::EFT_HitScan:
				FireHitScanWeapon();
				break;
			case EFireType::EFT_Shotgun:
				FireShotgun();
				break;
			}*/
		}
		StartFireTimer();
	}
}

void UCombatComponent::FireHitScanWeapon()
{

	//Local client fire


	if (EquippedWeapon && Player)
	{
		//HitTarget = EquippedWeapon->bUseScatter ? EquippedWeapon->TraceEndWithScatter(HitTarget) : HitTarget;
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{

}
