#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemManager.generated.h"


UCLASS()
class C_TEST_API AItemManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemManager();

	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	TMap<FString, TSubclassOf<class AWeapon>> WeaponList;





protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
