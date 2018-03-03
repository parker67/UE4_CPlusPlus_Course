// Copyright Daniel Parker 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Sets default values for this component's properties
	UGrabber();

private:
	// How far ahead of the player can we reach in cm	
	float Reach = 100.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;
	
	// Ray-cast and grab what is in reach
	void Grab();

	// Called when grab is released
	void Release();

	//find physics handle 
	void FindPhysicsHandleComponent();

	// Setup (assumed) attached input component
	void SetupInputComponent();

	// Return hit for first physics body in reach
	FHitResult GetFirstPhysicsBodyInReach() const;

	//Returns current start of reachline
	FVector GetReachLineStart() const;

	//Returns current end of reachline
	FVector GetReachLineEnd() const;

};