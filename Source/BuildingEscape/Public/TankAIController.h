// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tank.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class BUILDINGESCAPE_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
	
public:
	ATank* ControlledTank = nullptr;
	ATank* PlayerTank = nullptr;

	ATank* GetControlledTank() const;
	ATank* GetPlayerTank() const;

	void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
