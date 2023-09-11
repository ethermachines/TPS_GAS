// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPS_GASCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon.h"
#include "CombatComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TPSAnimInstance.h"
#include "Sound/SoundCue.h"


//////////////////////////////////////////////////////////////////////////
// ATPS_GASCharacter

ATPS_GASCharacter::ATPS_GASCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CameraDefaultFOV = 0.0f;
	CameraZoomedFOV = 60.0f;
	CameraCurrentFOV = 0.f;
	ZoomInterpSpeed = 20.f;

	//bIsFiring = false;
	bFireButtonPressed = false;

	//Combat Component Properties

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

}

void ATPS_GASCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (FollowCamera)
	{
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}

	SpawnDefaultWeapon();
}

//////////////////////////////////////////////////////////////////////////
// Input


void ATPS_GASCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPS_GASCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPS_GASCharacter::Look);

		//Firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ATPS_GASCharacter::FireButtonPressed);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ATPS_GASCharacter::FireButtonReleased);

		//Aiming
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Triggered, this, &ATPS_GASCharacter::AimingButtonPressed);
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &ATPS_GASCharacter::AimingButtonReleased);

	}

}

void ATPS_GASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat)
	{
		Combat->Player = this;
	}
}

void ATPS_GASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Interpolate FOV when Aiming
	OnAim(DeltaTime);
}


void ATPS_GASCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATPS_GASCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATPS_GASCharacter::AimingButtonPressed()
{
	bIsAiming = true;
}

void ATPS_GASCharacter::AimingButtonReleased()
{
	bIsAiming = false;
}

void ATPS_GASCharacter::OnAim(float DeltaTime)
{
	//aiming 
	if (bIsAiming)
	{
		// Interpolate to zoomed FOV
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraZoomedFOV,
			DeltaTime,
			ZoomInterpSpeed);

		//Lock player rotation to camera orientation
		bUseControllerRotationYaw = true;

		//Reduce player speed
		GetCharacterMovement()->MaxWalkSpeed = 250.f;
	}
	else
	{
		// Interpolate to default FOV
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraDefaultFOV,
			DeltaTime,
			ZoomInterpSpeed);

		bUseControllerRotationYaw = false;

		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	GetFollowCamera()->SetFieldOfView(CameraCurrentFOV);
}


void ATPS_GASCharacter::SpawnDefaultWeapon()
{
	//set weapon in blueprint first
	if (DefaultWeaponClass)
	{
		//spawn weapon in world
		AWeapon* DefaultWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass);

		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("RightHandSocket"));

		if (HandSocket)
		{
			HandSocket->AttachActor(DefaultWeapon, GetMesh());
		}

		Combat->EquipWeapon(DefaultWeapon);
		UE_LOG(LogTemp, Warning, TEXT("EquipWeapon on default"));
	}
}

AWeapon* ATPS_GASCharacter::GetEquippedWeapon()
{
	if (Combat == nullptr) return nullptr;
	return Combat->EquippedWeapon;
	//return EquippedWeapon;
}



void ATPS_GASCharacter::FireButtonPressed()
{
	bIsFiring = true;

	if (Combat)
		Combat->FireButtonPressed(true);

	bUseControllerRotationYaw = true;
	//FireWeapon();
	/*if (Combat)
	{
		Combat->FireButtonPressed(true);
	}*/
}

void ATPS_GASCharacter::FireButtonReleased()
{
	bIsFiring = false;

	if (Combat)
		Combat->FireButtonPressed(false);

	bUseControllerRotationYaw = false;
}

//void ATPS_GASCharacter::FireWeapon()
//{
//	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
//	if (BarrelSocket)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("BarrelSocket in"));
//		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());
//		FVector BeamEnd;
//
//		bool bBeamEnd = GetBeamEndLocation(SocketTransform.GetLocation(), BeamEnd);
//
//		if (bBeamEnd)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("Firing"));
//		}
//
//	}
//}
//
//bool ATPS_GASCharacter::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation)
//{
//	// Get current size of the viewport
//	FVector2D ViewportSize;
//	if (GEngine && GEngine->GameViewport)
//	{
//		GEngine->GameViewport->GetViewportSize(ViewportSize);
//	}
//
//	// Get screen space location of crosshairs
//	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
//	CrosshairLocation.Y -= 50.f;
//	FVector CrosshairWorldPosition;
//	FVector CrosshairWorldDirection;
//
//	// Get world position and direction of crosshairs
//	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
//		UGameplayStatics::GetPlayerController(this, 0),
//		CrosshairLocation,
//		CrosshairWorldPosition,
//		CrosshairWorldDirection);
//
//	if (bScreenToWorld) // was deprojection successful?
//	{
//		FHitResult ScreenTraceHit;
//		const FVector Start{ CrosshairWorldPosition };
//		const FVector End{ CrosshairWorldPosition + CrosshairWorldDirection * 50'000.f };
//
//		// Set beam end point to line trace end point
//		OutBeamLocation = End;
//
//		// Trace outward from crosshairs world location
//		GetWorld()->LineTraceSingleByChannel(
//			ScreenTraceHit,
//			Start,
//			End,
//			ECollisionChannel::ECC_Visibility);
//		if (ScreenTraceHit.bBlockingHit) // was there a trace hit?
//		{
//			// Beam end point is now trace hit location
//			OutBeamLocation = ScreenTraceHit.Location;
//		}
//
//		// Perform a second trace, this time from the gun barrel
//		FHitResult WeaponTraceHit;
//		const FVector WeaponTraceStart{ MuzzleSocketLocation };
//		const FVector WeaponTraceEnd{ OutBeamLocation };
//		GetWorld()->LineTraceSingleByChannel(
//			WeaponTraceHit,
//			WeaponTraceStart,
//			WeaponTraceEnd,
//			ECollisionChannel::ECC_Visibility);
//		if (WeaponTraceHit.bBlockingHit) // object between barrel and BeamEndPoint?
//		{
//			OutBeamLocation = WeaponTraceHit.Location;
//		}
//		else
//		{
//			DrawDebugSphere(GetWorld(), WeaponTraceHit.Location, 12.f, 12, FColor::Red);
//		}
//		return true;
//	}
//	return false;
//}


