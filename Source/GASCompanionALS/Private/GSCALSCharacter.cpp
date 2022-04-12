// Copyright 2021 Mickael Daniel. All Rights Reserved.


#include "GSCALSCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GSCTypes.h"
#include "Components/GSCAbilityQueueComponent.h"
#include "Components/GSCComboManagerComponent.h"
#include "Components/GSCCoreComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"

AGSCALSCharacter::AGSCALSCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup components
	GSCCoreComponent = CreateDefaultSubobject<UGSCCoreComponent>("GSCCoreComponent");
	GSCAbilityQueueComponent = CreateDefaultSubobject<UGSCAbilityQueueComponent>("GSCAbilityQueueComponent");
	GSCComboComponent = CreateDefaultSubobject<UGSCComboManagerComponent>("GSCComboComponent");

	// Setup sensible defaults
	bUseControllerRotationYaw = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	NetPriority = 4.0f;

	USkeletalMeshComponent* MeshComponent = GetMesh();

	MeshComponent->bEnableUpdateRateOptimizations = true;
	MeshComponent->bPropagateCurvesToSlaves = true;

	// Always tick Pose and refresh Bones!
	MeshComponent->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	// Setup ALS defaults
	Tags.Add(FName("ALS_Character"));
	bRightShoulder = true;
	bRagdollOnLand = true;
	MovementModel.DataTable = LoadObject<UDataTable>(nullptr, TEXT("/ALSV4_CPP/AdvancedLocomotionV4/Data/DataTables/MovementModelTable"));
	MovementModel.RowName = FName("Normal");
	MeshComponent->bUpdateJointsFromAnimation = true;
	MeshComponent->bEnableUpdateRateOptimizations = true;

	UCharacterMovementComponent* CMC = GetCharacterMovement();
	CMC->MaxAcceleration = 1500.f;
	CMC->BrakingFrictionFactor = 0.f;
	CMC->SetCrouchedHalfHeight(60.f);
	CMC->MinAnalogWalkSpeed = 25.f;
	CMC->bCanWalkOffLedgesWhenCrouching = true;
	CMC->AirControl = 0.15f;
	CMC->BrakingDecelerationFlying = 1000.f;
	CMC->NavAgentProps.bCanCrouch = true;
	CMC->NavAgentProps.bCanFly = true;

}

UAbilitySystemComponent* AGSCALSCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void AGSCALSCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerState* PS = GetPlayerState<APlayerState>())
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PS);
		if (!AbilitySystemComponent.IsValid())
		{
			return;
		}

		GSCCoreComponent->SetupOwner();
		GSCAbilityQueueComponent->SetupOwner();
		GSCComboComponent->SetupOwner();

		// Update parent's reference to AttributeSets
		// AttributeSets = PS->AttributeSets;

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initiating twice for heroes that have PlayerControllers.
		// TODO: ...
		// GSCCoreComponent->SetupAbilityActor(AbilitySystemComponent.Get(), PS, this);
	}
}

void AGSCALSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind player input to the AbilitySystemComponent. Also called in OnRep_PlayerState because of a potential race condition.
	BindASCInput();
}

void AGSCALSCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (APlayerState* PS = GetPlayerState<APlayerState>())
	{
		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PS);
		if (!AbilitySystemComponent.IsValid())
		{
			return;
		}

		// Setup owner again on components to update ASC ref
		GSCCoreComponent->SetupOwner();
		GSCAbilityQueueComponent->SetupOwner();
		GSCComboComponent->SetupOwner();

		// Update parent's reference to AttributeSets
		// AttributeSets = PS->AttributeSets;

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		// Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.
		BindASCInput();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		// TODO: ...
		// GSCCoreComponent->InitializeAttributes();
	}
}

void AGSCALSCharacter::BindASCInput()
{
	if (bASCInputBound)
	{
		return;
	}

	if (AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		// TOOD: ...
		// AbilitySystemComponent->BindAbilityActivationToInputComponent(
		// 	InputComponent,
		// 	FGameplayAbilityInputBinds(FString("ConfirmTarget"),
		// 	FString("CancelTarget"),
		// 	FString("EGSCAbilityInputID"),
		// 	static_cast<int32>(EGSCAbilityInputID::Confirm),
		// 	static_cast<int32>(EGSCAbilityInputID::Cancel))
		// );

		bASCInputBound = true;
	}
}
