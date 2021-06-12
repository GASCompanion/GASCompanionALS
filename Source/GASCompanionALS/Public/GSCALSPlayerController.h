// Copyright 2021 Mickael Daniel. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/GSCPlayerController.h"
#include "GSCALSPlayerController.generated.h"

class AGSCALSCharacter;

/**
 *
 */
UCLASS()
class GASCOMPANIONALS_API AGSCALSPlayerController : public AGSCPlayerController
{
	GENERATED_BODY()

public:

	AGSCALSPlayerController();

	/** Main character reference */
	UPROPERTY(BlueprintReadOnly, Category="Fires of Old|Player Controller")
	AGSCALSCharacter* OwnerCharacter = nullptr;

	virtual void OnRep_Pawn() override;

	virtual void CreateHUD() override;
protected:

	virtual void BeginPlayingState() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	void SetupCamera() const;
	void SetupDebugInputs() const;
};
