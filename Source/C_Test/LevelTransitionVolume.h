
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransitionVolume.generated.h"

UCLASS()
class C_TEST_API ALevelTransitionVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelTransitionVolume();


	UPROPERTY(VisibleAnywhere, Category = "Component")//volume per il caricamento livelli
	class UBoxComponent* BoxComponent;
	
	UBillboardComponent* BillBordComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transition")
	FName TransitionLevelName;

protected:
	virtual void BeginPlay() override;

	
public:	
	// Called every frame
	//virtual void Tick(float DeltaTime);
	
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
