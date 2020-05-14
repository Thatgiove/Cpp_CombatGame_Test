// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ColliderMovementComponent.h"

// Sets default values
ACollider::ACollider()
{

	CameraRotation = FVector2D(0.f, 0.f); // inizializzo vettore 2d per la rotazione

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")); //root -- eredita da actor

	//sphere collider
	//crea il comp sphere
	//attacca lo sphere comp  al root compon
	//ricordati di importare l'header
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
	//setta alcune prop dello sphere component
	SphereComponent->SetSphereRadius(50.f); //radius
	SphereComponent->SetCollisionProfileName(TEXT("Pawn")); // collision preset


	//static mesh
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	

	//springArm + prop
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetRelativeRotation(FRotator(-45.f,0,0));
	SpringArmComponent->TargetArmLength = 400;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 3.f;


	//camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(GetSpringArmComponent(), USpringArmComponent::SocketName);

	//mov component
	OurMovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("ColliderMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent; //a chi vogliamo agganciarlo

	//possess
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ACollider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	//motazione dell'actor sull'asse z -- yaw
	FRotator NewPawnRotation = GetActorRotation();
	NewPawnRotation.Yaw += CameraRotation.X;
	SetActorRotation(NewPawnRotation);


	FRotator NewSpringArmRotator = SpringArmComponent->GetComponentRotation(); // prendo il component rotation dello spring arm
	NewSpringArmRotator.Pitch = FMath::Clamp(NewSpringArmRotator.Pitch += CameraRotation.Y,-80.f,-15.f); // setto la rotazione sul pitch con il clamo
	SpringArmComponent->SetWorldRotation(NewSpringArmRotator);


}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACollider::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACollider::MoveRight);



	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &ACollider::CameraYaw);
	PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &ACollider::CameraPitch);

}



void  ACollider::MoveForward(float input)
{
	FVector Forward = GetActorForwardVector();

	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(input * Forward);
		//AddMovementInput(input * Forward);
	}

}

void  ACollider::MoveRight(float input) 
{
	FVector Right = GetActorRightVector();
	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(input * Right);
		//AddMovementInput(input * Right);
	}
	
}


//override del metodo gia presente in pawn movement
UPawnMovementComponent* ACollider::GetMovementComponent() const
{
	return OurMovementComponent;
}


void ACollider::CameraYaw(float value)
{
	CameraRotation.X = value;
}
void ACollider::CameraPitch(float value)
{
	CameraRotation.Y = value;
}