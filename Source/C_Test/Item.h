
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class C_TEST_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Item")
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UStaticMeshComponent* StaticMeshComponent;

	/*particle system per gli effetti permanenti*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UParticleSystemComponent* ParticleSystemComponent;

	/*particle system Creato con overlap*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UParticleSystem* ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class USoundCue* SoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bIsRotating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float RotationRate;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
	virtual void OnOverlapBegin(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

	UFUNCTION()
	virtual void OnOverlapEnd(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);
};
