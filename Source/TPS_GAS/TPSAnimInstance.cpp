// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAnimInstance.h"
#include "TPS_GASCharacter.h"
#include "Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTPSAnimInstance::UpdateAnimationProperties(float Delta)
{
	if (PlayerCharacter)
		PlayerCharacter = Cast<ATPS_GASCharacter>(TryGetPawnOwner());

	//Retrieve and assign character velocity magnitude to speed;
	if (PlayerCharacter)
	{
		FVector Velocity = FVector(PlayerCharacter->GetVelocity());
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsInAir = PlayerCharacter->GetCharacterMovement()->IsFalling();

		if (PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f)
			bIsAccelerating = true;
		else
			bIsAccelerating = false;

		bIsAiming = PlayerCharacter->GetAiming();

		FRotator AimRotation = PlayerCharacter->GetActorRotation();

		Direction = CalculateDirection(Velocity, AimRotation);

		const FVector AimDirWS = PlayerCharacter->GetBaseAimRotation().Vector();
		const FVector AimDirLS = PlayerCharacter->ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
		const FRotator AimRotLS = AimDirLS.Rotation();

		AimOffset = AimRotLS.Pitch;

		EquippedWeapon = PlayerCharacter->GetEquippedWeapon();
		

		/*FRotator AimRotation = PlayerCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(PlayerCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;*/

		//if weapon is equipped
		if (EquippedWeapon && EquippedWeapon->GetWeaponMesh() && PlayerCharacter->GetMesh())
		{
			LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
			FVector OutPosition;
			FRotator OutRotation;
			PlayerCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
			LeftHandTransform.SetLocation(OutPosition);
			LeftHandTransform.SetRotation(FQuat(OutRotation));
		}

	}

}

void UTPSAnimInstance::NativeInitializeAnimation()
{
	PlayerCharacter = Cast<ATPS_GASCharacter>(TryGetPawnOwner());
}
