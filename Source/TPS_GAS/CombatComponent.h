// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

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

	FVector HitTarget;

	bool bFireButtonPressed;

	FTimerHandle FireTimer;
	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire();

	void SetAiming(bool bIsAiming);

	void Fire();
	void FireHitScanWeapon();

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	//Aiming stuff possibly
	/*bool bAiming = false;
	bool bAimButtonPressed = false;*/

protected:

	void FireButtonPressed(bool bPressed);

		
};
