
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class C_TEST_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	AFloatingPlatform();

	/*mettere collider alla SM*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	FVector StartPoint;

	/*MakeEditWidget = posso manipolare questo oggetto nell'editor del livello*/
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"))
	FVector EndPoint;

	/*velocità di interpolazione*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	float InterpTime;

	FTimerHandle InterpTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platform")
	bool bIsInterping;

	float Distance;

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;
	void ToggleInterping();
	void SwapVectors(FVector& VectorA, FVector& VectorB);
};
