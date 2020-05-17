#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"


USTRUCT(BlueprintType)
struct FCharacterElementsToSave
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float Health;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float Stamina;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	float MaxStamina;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	int32 GoldCounter;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FVector Location;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FRotator Rotation;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FString LevelName;
	UPROPERTY(VisibleAnywhere, Category = "SaveGameData")
	FString WeaponName;
};


UCLASS()
class C_TEST_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:	
	UMySaveGame();

	UPROPERTY(VisibleAnywhere, Category="Save")
	FString SlotName;
	
	UPROPERTY(VisibleAnywhere, Category = "Save")
	uint32 SlotIndex;

	UPROPERTY(VisibleAnywhere, Category = "Save")
	FCharacterElementsToSave CharacterStats;
	
};
