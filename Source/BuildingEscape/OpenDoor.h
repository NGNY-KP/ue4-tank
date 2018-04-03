// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void UpdateTriggerStatus(ATriggerVolume* Trigger, bool &TriggerProp);

	UPROPERTY(BlueprintAssignable)
		FOnOpenRequest OnOpen;
	UPROPERTY(BlueprintAssignable)
		FOnCloseRequest OnClose;

private:

	// No longer used, kept for reference
	//UPROPERTY(EditAnywhere)
	//	float RequiredTriggerMass = 30.f;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* TrigRight = nullptr;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* TrigLeft = nullptr;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* TrigClose = nullptr;

	AActor* ActorThatOpens = nullptr;
	AActor* Owner = GetOwner();

	bool RightIsTriggered;
	bool LeftIsTriggered;
	bool CloseIsTriggered;

	// No longer used, kept for reference
	// float GetTotalMassOfActorsOnTrigger(ATriggerVolume*);
};
