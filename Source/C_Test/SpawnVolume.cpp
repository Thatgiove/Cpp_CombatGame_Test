#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Critter.h"
#include "Enemy.h"
#include "AIController.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
}


void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();



}


void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ASpawnVolume::GetSpawnPoint()
{
	/*funzione che ritorna la grandezza del box*/
	FVector Extent = SpawningBox->GetScaledBoxExtent();

	//il centro del box
	FVector Origin = SpawningBox->GetComponentLocation();

	/*questa funzione della UKismetMathLibrary
	prende in input due vettori e ritorna un punto casuale del box
	*/
	return UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	
}
TSubclassOf<AActor> ASpawnVolume::GetSpawnActor()
{
	if (EnemyList.Num() > 0)
	{
		uint32 random = FMath::RandRange(0, EnemyList.Num()-1);
		return EnemyList[random];
	}else
		return nullptr;
}

void ASpawnVolume::SpawnOurPawn_Implementation(UClass* OurPawnToSpawn, const FVector& LocationOfPawn)
{
	if (OurPawnToSpawn)
	{
		UWorld* World = GetWorld();
		//parametro dell'actor che deve essere spawnato
		FActorSpawnParameters ActorSpawnParameters;

		if (World)
		{
			AActor* Actor = World->SpawnActor<AActor>(OurPawnToSpawn, LocationOfPawn, FRotator(0), ActorSpawnParameters);
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy)
			{
				//catiamo actor a tipo Enemy e gli assegnamo un AIController
				Enemy->SpawnDefaultController();

				AAIController* AICont = Cast<AAIController>(Enemy->GetController());
				if (AICont)
				{
					Enemy->AIController = AICont;
				}
			}


				//spawn actor del tipo indicato
				//ACritter* CritterSpawned = World->SpawnActor<ACritter>(OurPawnToSpawn, LocationOfPawn, FRotator(0), ActorSpawnParameters);
			}
		}
			
	}


