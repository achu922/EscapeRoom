// Copyright Austin Chu 2021



#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "OpenDoor.h"


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

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the OpenDoor component, but no pressureplate set"), *GetOwner()->GetName());
	}

	ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpen))
	{
	OpenDoor(DeltaTime);
	//Counter to check once door is opened
	doorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		//If counter - counter of last time door was open is greater, delay close door.
		if(GetWorld()->GetTimeSeconds() - doorLastOpened > doorCloseDelay)
		{
			
			CloseDoor(DeltaTime);	
		}
	}

}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	// UE_LOG(LogTemp , Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString()); 
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);

	//Calculating currant yaw using Lerp
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime*doorOpenSpeed);
	//FRotator to get Rotation of door
	FRotator RotateDoor = GetOwner()->GetActorRotation();
	//Rotating Yaw
	RotateDoor.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(RotateDoor);
}
void UOpenDoor::CloseDoor(float DeltaTime)
{
	// UE_LOG(LogTemp , Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString()); 
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw);

	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime*doorCloseSpeed);
	FRotator RotateDoor = GetOwner()->GetActorRotation();
	RotateDoor.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(RotateDoor);
}