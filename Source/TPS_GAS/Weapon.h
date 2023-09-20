// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Idle UMETA(DisplayName = "Idle State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),
	EWS_Reloading UMETA(DisplayName = "Reloading"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EFireType : uint8
{
	EFT_HitScan UMETA(DisplayName = "Hit Scan Weapon"),
	EFT_Projectile UMETA(DisplayName = "Projectile Weapon"),
	EFT_Shotgun UMETA(DisplayName = "Shotgun Weapon"),

	EFT_MAX UMETA(DisplayName = "DefaultMAX")
};

class USphereComponent;

UCLASS()
class TPS_GAS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	//Weapon Fire Logic

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BulletsPerMinute;

	/*void StartFire();

	void StopFire();

	bool CanFire() const;*/

public:
	virtual void Fire(const FVector& HitTarget);

	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay = .15f;


private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USphereComponent* AreaSphere;

	UPROPERTY(VisibleAnywhere)
	EWeaponState WeaponState;

	EWeaponState CurrentState;

	/*void DetermineWeaponState();

	virtual void HandleFiring();*/

	bool bWantsToFire;

	float LastFireTime;

	/* Time between shots for repeating fire */
	float TimeBetweenShots;

	//Effects

	/*UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* EquipSound;*/

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* MuzzleFX;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EquipAnim;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FireAnim;

	UPROPERTY(Transient)
	UParticleSystemComponent* MuzzleEffect;

	/*UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticles;*/

	UPROPERTY(EditDefaultsOnly)
	FName MuzzleAttachPoint;

	bool bPlayingFireAnim;

public:

	FORCEINLINE EWeaponState GetCurrentState() const { return CurrentState; }
	FORCEINLINE void SetWeaponState(EWeaponState State) { CurrentState = State; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

};
