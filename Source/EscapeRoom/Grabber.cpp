// Copyright Austin Chu 2021
#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// PhysicsHandle->ReleaseComponent();
	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// UE_LOG(LogTemp, Warning, TEXT("Grabber Working"));

	//Check For Physics Handle Component
	FindingPhysicsHandler();
	//InputComponent, binding action with grab and grab release
	SetInputComponent();

}
void UGrabber::FindingPhysicsHandler()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//Physics Handle found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR: No Physics Handle Component on %s"), *GetOwner()->GetName());
	}

}
void UGrabber::SetInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Input Component Found on %s!"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::GrabRelease);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component Missing!"));
	}

}
void UGrabber::Grab()
{

	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed"));
	// Get Players Viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * reach;
	//Raycast when key is pressed (F) and Reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	
	//If hit, then attach physics handle
	if(HitResult.GetActor())
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);

}
void UGrabber::GrabRelease()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released"));

	//Remove Physics Handle
}
//----------------------------------------------------------
// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//------------------------------------------------------------
	//Logs Location and Rotation
	// UE_LOG(LogTemp, Warning, TEXT("Location: %s | Rotation: %s"), 
	// 	*PlayerViewPointLocation.ToString(), 
	// 	*PlayerViewPointRotation.ToString()
	// );
	//------------------------------------------------------------
	//DEBUG LINE FOR RAYCASTING
	// FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * reach;
	// DrawDebugLine(
	// 	GetWorld(),
	// 	PlayerViewPointLocation,
	// 	LineTraceEnd,
	// 	FColor(0,255,0),
	// 	false,
	// 	0.f,
	// 	0,
	// 	5.f
	// );
	//------------------------------------------------------------
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//If the Physics Handle is attached
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * reach;
	if(!PhysicsHandle){
		return; //To catch null exception
	}
	if(PhysicsHandle->GrabbedComponent)
	{
		//Move Object we are holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
		//Remove Object currently handling
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	

	// Get Players Viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * reach;

	FHitResult Hit;
	// Ray Cast out to a certain distance (reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType	(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams

		
	);
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit){
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *(ActorHit->GetName()));
	}

	return Hit;
}