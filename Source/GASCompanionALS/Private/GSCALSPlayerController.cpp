// Copyright 2021 Mickael Daniel. All Rights Reserved.


#include "GSCALSPlayerController.h"

#include "GSCALSCharacter.h"
#include "Character/ALSPlayerCameraManager.h"
#include "Components/ALSDebugComponent.h"
#include "UI/GSCUWHud.h"

AGSCALSPlayerController::AGSCALSPlayerController()
{
	PlayerCameraManagerClass = StaticLoadClass(AALSPlayerCameraManager::StaticClass(), nullptr, TEXT("/ALSV4_CPP/AdvancedLocomotionV4/Blueprints/CameraSystem/ALS_PlayerCameraManager.ALS_PlayerCameraManager_C"));
}

void AGSCALSPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	OwnerCharacter = Cast<AGSCALSCharacter>(GetPawn());
	SetupCamera();
}

void AGSCALSPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		SetupDebugInputs();
	}
}

void AGSCALSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	OwnerCharacter = Cast<AGSCALSCharacter>(InPawn);
	if (!IsRunningDedicatedServer())
	{
		// Servers want to setup camera only in listen servers.
		SetupCamera();
	}

	SetupDebugInputs();
}

void AGSCALSPlayerController::SetupCamera() const
{
	// Call "OnPossess" in Player Camera Manager when possessing a pawn
	AALSPlayerCameraManager* CameraManager = Cast<AALSPlayerCameraManager>(PlayerCameraManager);
	if (OwnerCharacter && CameraManager)
	{
		CameraManager->OnPossess(OwnerCharacter);
	}
}

void AGSCALSPlayerController::SetupDebugInputs() const
{
	// Bind inputs for debugging
	if (OwnerCharacter)
	{
		UActorComponent* Comp = OwnerCharacter->GetComponentByClass(UALSDebugComponent::StaticClass());
		if (Comp)
		{
			UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(Comp);
			if (InputComponent && DebugComp)
			{
				// TODO: Use InputAction
				InputComponent->BindKey(FKey("Tab"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleHud);
				// InputComponent->BindKey(FKey("L"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleHud);
				InputComponent->BindKey(FKey("V"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleDebugView);
				InputComponent->BindKey(FKey("T"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleTraces);
				InputComponent->BindKey(FKey("Y"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleDebugShapes);
				InputComponent->BindKey(FKey("U"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleLayerColors);
				InputComponent->BindKey(FKey("I"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleCharacterInfo);
				InputComponent->BindKey(FKey("M"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleDebugMesh);
				// InputComponent->BindKey(FKey("Z"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleSlomo);
				InputComponent->BindKey(FKey("K"), EInputEvent::IE_Pressed, DebugComp, &UALSDebugComponent::ToggleSlomo);
			}
		}
	}
}
