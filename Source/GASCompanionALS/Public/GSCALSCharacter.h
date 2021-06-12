// Copyright 2021 Mickael Daniel. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/ALSCharacter.h"
#include "AbilitySystemInterface.h"
#include "Core/Interfaces/GSCCompanionInterface.h"
#include "GSCALSCharacter.generated.h"

/**
 *
 */
UCLASS()
class GASCOMPANIONALS_API AGSCALSCharacter : public AALSCharacter, public IAbilitySystemInterface, public IGSCCompanionInterface
{
	GENERATED_BODY()

public:
	AGSCALSCharacter(const FObjectInitializer& ObjectInitializer);

	/** Companion Core Component */
	UPROPERTY(Category = "GSC|Components", VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UGSCCoreComponent* GSCCoreComponent;

	/** Ability Queue Component */
	UPROPERTY(Category = "GSC|Components", VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UGSCAbilityQueueComponent* GSCAbilityQueueComponent;

	/** Combo Manager Component */
	UPROPERTY(Category = "GSC|Components", VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UGSCComboManagerComponent* GSCComboComponent;
	// AttributeSets created in child classes
	UPROPERTY()
	TArray<const UAttributeSet*> AttributeSets;

	// Keep track of registered attribute sets to avoid adding it twice
	UPROPERTY()
	TArray<FString> RegisteredAttributeSetNames;

	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ~ IAbilitySystemInterface

	// Implement IGSCCompanionInterface
	virtual UGSCCoreComponent* GetCompanionCoreComponent() const override;
	virtual UGSCComboManagerComponent* GetComboManagerComponent() const override;
	virtual UGSCAbilityQueueComponent* GetAbilityQueueComponent() const override;
	virtual TArray<const UAttributeSet*> GetAttributeSets() const override;
	// ~ IGSCCompanionInterface

	// Only called on the Server. Calls before Server's AcknowledgePossession.
	virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Client only
	virtual void OnRep_PlayerState() override;

protected:

	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/**
	* Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race
	* condition. The PlayerController might call ClientRestart (which calls SetupPlayerInputComponent)
	* before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	*
	* Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart
	* so the Actor's InputComponent would be null in OnRep_PlayerState.
	*/
	void BindASCInput();
	bool bASCInputBound = false;
};
