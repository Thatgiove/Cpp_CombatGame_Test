/**
QUESTA CLASSE CREA UNA PIATTAFORMA CHE VA AVANTI E INDIETRO



*/
#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

AFloatingPlatform::AFloatingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	RootComponent = StaticMeshComponent;

	StartPoint = FVector(0);
	EndPoint = FVector(0);

	bIsInterping = false;

	InterpSpeed = 4.0f;
	InterpTime = 1.f;

}

void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	//all'inizio prende la location dell'actor di riferimento 
	StartPoint = GetActorLocation();
	EndPoint += StartPoint;
	bIsInterping = false;

	

	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);

	Distance = (EndPoint - StartPoint).Size();
}


void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsInterping)
	{
		FVector Actor_every_frames_Location = GetActorLocation();
		//creo vettore di interpolazione
		FVector interpolationVector = FMath::VInterpTo(Actor_every_frames_Location, EndPoint, DeltaTime, InterpSpeed);
		SetActorLocation(interpolationVector);

		float DistanceTraveled = (GetActorLocation() - StartPoint).Size();
		if (Distance - DistanceTraveled <= 1.f)
		{
			ToggleInterping();

			GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
			SwapVectors(StartPoint, EndPoint);
		}


	}
}

void AFloatingPlatform::ToggleInterping()
{
	this->bIsInterping = !this->bIsInterping;
}

void AFloatingPlatform::SwapVectors(FVector& VectorA, FVector& VectorB)
{
	FVector Temp = VectorA;
	VectorA = VectorB;
	VectorB = Temp;
}

