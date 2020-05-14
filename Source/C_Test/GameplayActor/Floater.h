// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater.generated.h"

UCLASS()
class C_TEST_API AFloater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloater();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ActorMeshComponent") //editabile nell'editor
	UStaticMeshComponent* StaticMesh; //puntatore a static mesh, per vederlo nelle BP bisogna costruirlo nel costruttore
	
	// posso manipolare il valore della variabile sulla bluprint
	// EditInstanceOnly = editabile solo nell'istanza	 
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "FloaterVariables") 
	FVector InitialPosition; //vettore zero
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "FloaterVariables")
	FVector PlacedLocation; //location dell'actor trascinato nel livello

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloaterVariables")
	FVector InitialDirection; //location dell'actor trascinato nel livello
	

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "FloaterVariables")
	FVector InitialForce; //forza da applicare all'oggetto fisico

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloaterVariables")
	FVector InitialTorque; 


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloaterVariables | Sine Parameter")
	float Amplitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloaterVariables | Sine Parameter")
	float TimeStretch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloaterVariables | Sine Parameter")
	float  C;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FloaterVariables")
	bool bInitializeFloaterLocation; //editaqbile a livello di blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVariables")
	bool bShouldFloat; //editaqbile ovunque

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	float RunningTime;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
