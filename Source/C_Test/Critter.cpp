// Fill out your copyright notice in the Description page of Project Settings.


#include "Critter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

// Sets default values
ACritter::ACritter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root_Component")); //root -- eredita da actor

	//crea il comp skeletal mesh
	//attacca lo skeletal mesh comp  al root component
	//ricordati di importare l'header
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh_Component"));
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());

	//crea e attacca cameraComponent alla root
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera_Component"));
	CameraComponent->SetupAttachment(GetRootComponent());
	CameraComponent->SetRelativeLocation(FVector(-300.f, 0, 300.f)); //set location
	CameraComponent->SetRelativeRotation(FRotator(-45.f, 0, 0)); //set rotation

	//riceve input da player 0
	//ricordati di impostare questo pawn come default nel gameMode
	//AutoPossessPlayer = EAutoReceiveInput::Player0;


	Velocity = FVector(0);
	
	MaxSpeed = 300.f;
}

// Called when the game starts or when spawned
void ACritter::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void ACritter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);




	FVector NewLocation = GetActorLocation() + (Velocity * DeltaTime); //moltiplica per deltaTime
	SetActorLocation(NewLocation);

}

// Called to bind functionality to input
void ACritter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//TEXT("MoveForward") ---> tipo di input defito il impostazioni inputsetting (W - S)
	//this --- > chi è soggetto al movimento
	// &ACritter::MoveForward --- > puntatore a funzione che esegue
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACritter::MoveForward); 
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACritter::MoveRight); 


}




void ACritter::MoveForward(float value) 
{
	Velocity.X = FMath::Clamp(value, -1.f, 1.f) * MaxSpeed; //--> clamp tra un minimo e max
}
void ACritter::MoveRight(float value)
{
	Velocity.Y = FMath::Clamp(value, -1.f, 1.f) * MaxSpeed; //--> clamp tra un minimo e max
}

