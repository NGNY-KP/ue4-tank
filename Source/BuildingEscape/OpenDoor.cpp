// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

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

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	RightIsTriggered = false;
	LeftIsTriggered = false;
}

void UOpenDoor::OpenDoor()
{
	AActor* Owner = GetOwner();

	//FRotator NewRotation = FRotator(0.0f, 90.0f, 0.0f);

	Owner->SetActorRotation(FRotator(0.0f, 30.0f, 0.0f));
	Owner->SetActorRotation(FRotator(0.0f, 60.0f, 0.0f));
	Owner->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (RightIsTriggered && LeftIsTriggered) {
		OpenDoor();
	} 
	else if (TrigRight->IsOverlappingActor(ActorThatOpens)) 
	{
		RightIsTriggered = true;
	} 
	else if (TrigLeft->IsOverlappingActor(ActorThatOpens))
	{
		LeftIsTriggered = true;
	}
	
}

