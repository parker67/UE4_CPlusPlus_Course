// Copyright Daniel Parker 2018

#include "Grabber.h"
#include "engine/World.h"
#include "Engine.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();

}

// look for attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{

	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle not attached to %s"),
			*(GetOwner()->GetName()))
	}
 }
// look for attached inout component (only appears at run time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
		InputComponent->BindAction("Switch", IE_Pressed, this, &UGrabber::Switch);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *(GetOwner()->GetName()))
	}
}

void UGrabber::Release() 
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"))
		if (!PhysicsHandle) { return; }
		PhysicsHandle->ReleaseComponent();
}

void UGrabber::Switch()
{
	UE_LOG(LogTemp, Warning, TEXT("Switch Pressed"))
	auto HitResult = CheckForSwitch();
	auto ActorHit = HitResult.GetActor();
	/// if hit something then attach a physics handle
	if (ActorHit)
	{
		FOutputDeviceNull ar;
		ActorHit->CallFunctionByNameWithArguments(TEXT("CustomFunction"), ar, NULL, true);
		SwitchesHit++; 
	} else {
		return;
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))

	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	/// if hit something then attach a physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocationWithRotation(
		ComponentToGrab, 
		NAME_None, // no bones needed
		ComponentToGrab->GetOwner()->GetActorLocation(),
		ComponentToGrab->GetOwner()->GetActorRotation()); //allow rotation
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if physics handle attached
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GetGrabbedComponent())
	{
		//move the object that were holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
	/// see what we hit

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{

	/// Setup query parameters

	/// Line Trace (Ray-cast) out to reach distance
	FHitResult LineTraceHit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	AActor* ActorHit = LineTraceHit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor hit is %s"),
			*(ActorHit->GetName())
		);
	}
	return LineTraceHit;
}

FHitResult UGrabber::CheckForSwitch() const
{

	/// Setup query parameters

	/// Line Trace (Ray-cast) out to reach distance
	FHitResult LineTraceHit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByChannel(
		OUT LineTraceHit,
		GetReachLineStart(),
		GetReachLineEnd(),
		ECC_Pawn,
		TraceParameters
	);
	AActor* ActorHit = LineTraceHit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor hit is %s"),
			*(ActorHit->GetName())
		);
	}
	return LineTraceHit;
}

int32 UGrabber::GetSwitchesHit()
{
	return SwitchesHit;
}

FVector UGrabber::GetReachLineStart() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}