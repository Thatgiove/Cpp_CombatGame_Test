// Fill out your copyright notice in the Description page of Project Settings.


#include "ColliderMovementComponent.h"

//overload della funzione in movementComponent
void UColliderMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime,  TickType, ThisTickFunction);

	// controlla se tutto è ancora valido altrimenti esco 
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	//prendi e pulisci il vettore dal ColliderPawn
	//il vettore non è piu grande di 1
	//moltiplicato per la velocita 150.f
	FVector DesideredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 150.f; 

	//constrolla che non sia zero
	if (!DesideredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		//muove il component
		SafeMoveUpdatedComponent(DesideredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		//se colpiamo qualcosa... scivoliamo sulla sua superficie
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(/*vettore*/DesideredMovementThisFrame,/*time*/ 1.f - Hit.Time, /*normale della superficie colpita*/Hit.Normal, Hit);
		}


	}

}