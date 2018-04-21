// Copyright Daniel Parker 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LampSwitch.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API ULampSwitch : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULampSwitch();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	AActor* Owner = nullptr;
		
	
};
