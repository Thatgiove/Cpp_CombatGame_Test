#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AMyTriggerVolume.generated.h"

UCLASS()
class C_TEST_API AAMyTriggerVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AAMyTriggerVolume();

	//box trigger
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category="Door Switch")
	class UBoxComponent* TriggerBox;
	
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Door Switch")
	class UStaticMeshComponent* FloorSwitch;
	
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Door Switch")
	class UStaticMeshComponent* Door;

	/*posizione iniziale della door*/
	UPROPERTY(BlueprintReadWrite, Category = "Door Switch")
	FVector InitialDoorLocation;

	/*posizione iniziale della switch*/
	UPROPERTY(BlueprintReadWrite, Category = "Door Switch")
	FVector InitialSwitchLocation;

	//struct necessaria per settare un timer
	FTimerHandle SwitchTimerHandle;

	//time in secondi
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Switch")
	float SwitchTime;

	
	bool bCharacterOnSwitch; 
protected:

	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	//funzione delegato deve essere UFUNCTION 
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult
	);

	UFUNCTION()
		void OnOverlapEnd(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex
		);
	
	
	/**
	BlueprintImplementableEvent -- Implementabile nella Blueprint
	non ho bisogno di definire questa funzione nel cpp 
	lo faccio direttamente nell'editor Blueprint

	Quindi in BP_TriggerVolume troverò un evento RaiseDoor
	in rosso -- i nodi rossi identificano gli eventi
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Door Switch")
	void RaiseDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Door Switch")
	void LowerDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Door Switch")
	void RaiseFloorSwitch();

	UFUNCTION(BlueprintImplementableEvent, Category = "Door Switch")
	void LowerFloorSwitch();

	/**
	BlueprintCallable = posso chiamare questa funzione dalla Blueprint
	ma la funzionalità sono scritte nel c++
	
	*/
	UFUNCTION(BlueprintCallable, Category = "Door Switch")
	void UpdateDoorLocation(float ZValue);
	
	
	void CloseDoor();
};
