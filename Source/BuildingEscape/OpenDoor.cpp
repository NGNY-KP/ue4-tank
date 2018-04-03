// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	RightIsTriggered = false;
	LeftIsTriggered = false;
	CloseIsTriggered = false;

	if (!TrigRight) { UE_LOG(LogTemp, Warning, TEXT("No TrigRight")); }
	if (!TrigLeft) { UE_LOG(LogTemp, Warning, TEXT("No TrigLeft")); }
	if (!TrigClose) { UE_LOG(LogTemp, Warning, TEXT("No TrigClose")); }

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::UpdateTriggerStatus(ATriggerVolume * Trigger, bool &TriggerProp)
{
	if (!ActorThatOpens) { return; }
	if (Trigger->IsOverlappingActor(ActorThatOpens)) 
	{
		TriggerProp = true;
	} 
}

/* No longer used, kept for reference
float UOpenDoor::GetTotalMassOfActorsOnTrigger(ATriggerVolume * Trigger)
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	Trigger->GetOverlappingActors(OUT OverlappingActors);
	
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on trigger"), *(Actor->GetName()));
	}

	return TotalMass;
}
*/

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!TrigRight) { return; }
	UpdateTriggerStatus(TrigRight, RightIsTriggered);
	if (!TrigLeft) { return; }
	UpdateTriggerStatus(TrigLeft, LeftIsTriggered);
	if (!TrigClose) { return; }
	UpdateTriggerStatus(TrigClose, CloseIsTriggered);

	if (CloseIsTriggered) {
		RightIsTriggered = false;
		LeftIsTriggered = false;
		OnClose.Broadcast();
	} 
	else if (RightIsTriggered && LeftIsTriggered) {
		OnOpen.Broadcast();
	} 
}