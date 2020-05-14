/**
	QUESTA CLASSE GENERA UN NEMICO IN UN POSIZIONE CASUALE DEL LIVELLO




*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class C_TEST_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	class UBoxComponent* SpawningBox;

	//creaa una dropdown con le classi specificate nel generic
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	class TSubclassOf<class ACritter> PawnToSpawn;

	//creaa una dropdown con le classi specificate nel generic
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TArray<TSubclassOf<class AActor>> EnemyList;



protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	//funzione che ritorna un vettore -- punto 
	//funzione pura = solo output
	UFUNCTION(BlueprintPure, Category="Spawning")
	FVector GetSpawnPoint();
	UFUNCTION(BlueprintPure, Category = "Spawning")
	TSubclassOf<AActor> GetSpawnActor();

	//BlueprintNativeEvent funzione ibrida -- funzionalita in blueprint e c++
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spawning")
	void SpawnOurPawn(UClass* OurPawnToSpawn, const FVector& LocationOfPawn);
};
