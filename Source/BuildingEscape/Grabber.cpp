// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	CheckPhysicsHandler();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing InputComponent"), *GetOwner()->GetName());
	}
}

void UGrabber::CheckPhysicsHandler()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) // If owner has no physics handle component
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing PhysicsHandleComponent"), *GetOwner()->GetName());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	/// Line-trace out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	FTwoVectors TracePoints = GetLineTracePoints();
	GetWorld()->LineTraceSingleByObjectType
	(
		OUT HitResult,
		TracePoints.v1,
		TracePoints.v2,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	/// See what we hit
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit %s"), *(ActorHit->GetName()));
	}

	return HitResult;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabbed"));

	/// Try to reach grabbable object
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// If it hits, attach a physics handle
	if (ActorHit) 
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
		
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Released"));
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

FTwoVectors UGrabber::GetLineTracePoints() const
{
	FVector StartLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT StartLocation,
		OUT PlayerViewPointRotation
	);

	FVector EndLocation = StartLocation + PlayerViewPointRotation.Vector() * Reach;
	return FTwoVectors(StartLocation, EndLocation);
	
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// if the physics handle is attached
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetLineTracePoints().v2); /// move the held object
	}
}