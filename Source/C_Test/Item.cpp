#include "Item.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	RootComponent = SphereComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	ParticleSystemComponent->SetupAttachment(GetRootComponent());


	bIsRotating = false;

	RotationRate = 45.f;
}


void AItem::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRotating)
	{
		/*Aggiunge una rotazione costante*/
		FRotator ActorRotation = GetActorRotation();
		ActorRotation.Yaw += DeltaTime * RotationRate;
		SetActorRotation(ActorRotation);
	}

}

void AItem::OnOverlapBegin
(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult
)
{
	//UE_LOG(LogTemp, Warning, TEXT("OverlapBegin::ITEM_MAIN"));
	if (ParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, GetActorLocation(),GetActorRotation(),true);
	
	}
	if (SoundCue)
	{
		UGameplayStatics::PlaySound2D(this, SoundCue);
	}

	

}

void AItem::OnOverlapEnd
(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex
)
{
	//UE_LOG(LogTemp, Warning, TEXT("OverlapEnd::ITEM_MAIN"));
}

