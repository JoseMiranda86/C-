#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

UOpenDoor::UOpenDoor()
{

	PrimaryComponentTick.bCanEverTick = true;

}

void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner() -> GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle = OpenAngle + InitialYaw;

	FindpressurePlate();
	FindAudioComponent();

}

void UOpenDoor::FindpressurePlate()
{
	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the open door component on it, but no pressureplates set!"), *GetOwner() -> GetName());
	}
}

void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > 50.f)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld() -> GetTimeSeconds();
	}
	else
	{
		if (GetWorld() -> GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation = GetOwner() -> GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner() -> SetActorRotation(DoorRotation);

	CloseDoorSound = false;
	if (!AudioComponent) {return;}
	if (!OpenDoorSound)
	{
		AudioComponent -> Play();
		OpenDoorSound = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation = GetOwner() -> GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner() -> SetActorRotation(DoorRotation);

}