// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(Sandbox, Log, All);
#define LOG(x, ...) UE_LOG(Sandbox, Log, TEXT(x), __VA_ARGS__)

class ATPS_GASCharacter;
class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_GAS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

	friend class ATPS_GASCharacter;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY()
	ATPS_GASCharacter* Player;

	AWeapon* EquippedWeapon;

	void EquipWeapon(AWeapon* WeaponToEquip);

	//Firing Logic
	bool bFireButtonPressed;
	void Fire();
	void FireHitScanWeapon(FVector& TraceHitLine);
	FVector TraceHit;
	FVector HitTarget;

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	//Aiming logic
	//bool bAiming = false;
	bool bAimButtonPressed;

	//Fire Timer logic
	FTimerHandle FireTimer;

	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire();
	bool bCanFire = true;



protected:

	void FireButtonPressed(bool bPressed);
	void AimButtonPressed(bool bIsAiming);

public:

	FORCEINLINE bool GetAiming() const { return bAimButtonPressed; }
};
