// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "PickUp.generated.h"

/**
 * 
 */
UCLASS()
class C_TEST_API APickUp : public AItem
{
	GENERATED_BODY()
	

public:
	APickUp();



	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	//Questa funzione scatena un evento che viene gestito nella BP
	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void OnPickUp(class AMainCharacter* Main);
};
