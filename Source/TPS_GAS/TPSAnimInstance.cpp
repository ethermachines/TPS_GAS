// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAnimInstance.h"
#include "TPS_GASCharacter.h"
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
		

		/*FRotator AimRotation = PlayerCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(PlayerCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;*/


	}

}

void UTPSAnimInstance::NativeInitializeAnimation()
{
	PlayerCharacter = Cast<ATPS_GASCharacter>(TryGetPawnOwner());
}
