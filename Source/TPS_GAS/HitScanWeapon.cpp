// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "TPS_GASCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	//UE_LOG(LogTemp, Warning, TEXT("Hitscan Fire"));

	/*APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();*/

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket) //&& InstigatorController
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();
		FVector End = Start + (HitTarget - Start) * 1.25f;

		FHitResult FireHit;
		UWorld* World = GetWorld();
		if (World)
		{
			World->LineTraceSingleByChannel(FireHit, Start, End, ECollisionChannel::ECC_Visibility);
		}

		if (FireHit.bBlockingHit)
		{
			ATPS_GASCharacter* Character = Cast<ATPS_GASCharacter>(FireHit.GetActor());
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
			DrawDebugPoint(GetWorld(), FireHit.Location, 5.0f, FColor::Red, false, 2.0f);
			if (Character)
			{
				//UGameplayStatics::ApplyDamage(Character, Damage, InstigatorController, this, UDamageType::StaticClass());
				UE_LOG(LogTemp, Warning, TEXT("Apply Damage"));
			}
			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticles, FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
			}
		}

	}
}
