// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSAnimInstance.generated.h"

class ATPS_GASCharacter;
class AWeapon;

UCLASS()
class TPS_GAS_API UTPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float Delta);

	virtual void NativeInitializeAnimation() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	ATPS_GASCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsFiring;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float AimOffset;
	
	//IK hands on weapon

	AWeapon* EquippedWeapon;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FTransform LeftHandTransform;

public:
	
};
