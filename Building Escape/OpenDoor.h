// Copyright Nicolas Diel

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


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
	void FindAudioComponent();
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void FindPressurePlate() const;
	float TotalMassOfActors() const;

private:
	float InitialYaw;
	float CurrentYaw;	
	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 50.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.2f;

	UPROPERTY(EditAnywhere)
	float OpenAngle = -90.f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 20.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	// Tracks whether the sound has been played.
	bool OpenDoorSound = false;
	bool CloseDoorSound = true;
};
