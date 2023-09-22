// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "TPS_GASCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CurrentState = EWeaponState::EWS_Idle;

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	//hasauthority for multi
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Fire(const FVector& HitTarget)
{

	//AEnemy* HitEnemy = Cast<AEnemy>(HitTarget.GetActor());
	//if (HitEnemy)
	//{
	//	UGameplayStatics::ApplyDamage(HitEnemy, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
	//	//UE_LOG(LogTemp, Warning, TEXT("Applied Damage"));
	//}

	//spend ammo
	//UE_LOG(LogTemp, Warning, TEXT("Weapon Fire"));

	/*APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket && InstigatorController)
	{
		UE_LOG(LogTemp, Warning, TEXT("In Muzzle Mesh and Controller"));
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();
		FVector End = Start + (HitTarget - Start) * 1.25f;

		FHitResult FireHit;
		UWorld* World = GetWorld();
		if (World)
		{
			UE_LOG(LogTemp, Warning, TEXT("In World"));
			World->LineTraceSingleByChannel(FireHit, Start, End, ECollisionChannel::ECC_Visibility);
		}

		if (FireHit.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("In FireHit"));
			ATPS_GASCharacter* Character = Cast<ATPS_GASCharacter>(FireHit.GetActor());
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
			DrawDebugPoint(GetWorld(), FireHit.Location, 5.0f, FColor::Red, false, 2.0f);
			if (Character)
			{
				UGameplayStatics::ApplyDamage(Character, 20.0f, InstigatorController, this, UDamageType::StaticClass());
			}
			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticles, FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
			}
		}

	}*/
}

