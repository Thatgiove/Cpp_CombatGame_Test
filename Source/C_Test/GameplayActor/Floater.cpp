// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloater::AFloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CUSTOM_STATIC_MESH"));

	InitialPosition.Z = FMath::FRandRange(0.f,500.f);

	PlacedLocation = FVector(0);
	InitialDirection = FVector(0);

	InitialForce = FVector(8000000.f, 0, 0);
	InitialTorque = FVector(8000000.f, 0, 0);
	
	bInitializeFloaterLocation = false;
	bShouldFloat = true;



	RunningTime = 0.f;
	Amplitude = 1.f;
	TimeStretch = 1.f;
	C = 1.f;
}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();

	PlacedLocation = GetActorLocation(); //prende la location dell'actor
	FRotator Rotator = FRotator(0); //rotatotor struct per le rotazioni pitch roll yaw
	if(bInitializeFloaterLocation)
		SetActorLocation(PlacedLocation);
	


	StaticMesh->AddForce(InitialForce); //sfrutto le funz di static mesh ma devo imporatre l'header
	StaticMesh->AddTorque(InitialTorque); //movimento circolare


}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldFloat) 
	{
		//FHitResult HitResult; // struct
		//AddActorLocalOffset(InitialDirection,false, &HitResult); //passato come puntatore il secondo parametro bSweep rileva le collisioni anche se il motere fisico è off
		FVector NewLocation = GetActorLocation();

		NewLocation.Z = NewLocation.Z + Amplitude * FMath::Sin(TimeStretch * RunningTime) ;
		NewLocation.Y = NewLocation.Y + Amplitude * FMath::Cos(TimeStretch * RunningTime) ; 

		SetActorLocation(NewLocation);
		RunningTime += DeltaTime; //ad ogni tick aumenta il valore di RunningTime aggiungendo il tempo in secondi tra un frame e l'altro
	
	}

}

