// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "TPS_GASCharacter.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

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

	//if (Player)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Got Weapon"));
	//	//EquippedWeapon = Player->GetEquippedWeapon();
	//}

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

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	EquippedWeapon = WeaponToEquip;
	//EquippedWeapon->SetOwner(Player);
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
	return true;
}


void UCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if (Player && bFireButtonPressed)
	{
		//UE_LOG(LogTemp, Warning, TEXT("In FirePress (Combat comp)"));
		//Player->PlayFireMontage(bAiming);
		FireHitScanWeapon(HitTarget);
	}
}

void UCombatComponent::Fire()
{

	//FireHitScanWeapon(TraceHit);

	//if (CanFire())
	//{
	//	bCanFire = false;
	//	if (EquippedWeapon)
	//	{
	//		//CrosshairShootingFactor = .75f;

	//		FireHitScanWeapon();
	//		/*switch (EquippedWeapon->FireType)
	//		{
	//		case EFireType::EFT_Projectile:
	//			FireProjectileWeapon();
	//			break;
	//		case EFireType::EFT_HitScan:
	//			FireHitScanWeapon();
	//			break;
	//		case EFireType::EFT_Shotgun:
	//			FireShotgun();
	//			break;
	//		}*/
	//	}
	//	StartFireTimer();
	//}
}

void UCombatComponent::FireHitScanWeapon(FVector& TraceHitLine)
{

	//Local client fire

	//UE_LOG(LogTemp, Warning, TEXT("In FireHitScan before Equipped Check"));

	if (EquippedWeapon && Player)
	{
		//UE_LOG(LogTemp, Warning, TEXT("In FireHitScan before Equipped fire"));
		//HitTarget = EquippedWeapon->bUseScatter ? EquippedWeapon->TraceEndWithScatter(HitTarget) : HitTarget;
		EquippedWeapon->Fire(TraceHitLine);
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		FVector End = Start + CrosshairWorldDirection * 80000.0f;

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility
		);
		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
		}
		else
		{
			DrawDebugSphere(
				GetWorld(),
				TraceHitResult.ImpactPoint,
				12.f,
				12,
				FColor::Red
			);
		}
	}
}
