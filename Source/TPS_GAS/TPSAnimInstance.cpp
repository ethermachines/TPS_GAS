// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAnimInstance.h"
#include "TPS_GASCharacter.h"
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
	}

}

void UTPSAnimInstance::NativeInitializeAnimation()
{
	PlayerCharacter = Cast<ATPS_GASCharacter>(TryGetPawnOwner());
}
