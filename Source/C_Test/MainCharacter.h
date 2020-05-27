// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"


UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_NORMAL UMETA(DyplayName = "Normal"),
	EMS_SPRINTING UMETA(DyplayName = "Sprinting"),
	EMS_DEATH UMETA(DyplayName = "Death"),
	
	EMS_MAX UMETA(DyplayName = "DefaultMax")
};


UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DyplayName = "Normal"),
	ESS_Below_Minum UMETA(DyplayName = "Below minumum"),
	ESS_Exhausted UMETA(DyplayName = "Exhausted"),
	ESS_Exhausted_Recovering UMETA(DyplayName = "Exhausted Recovering"),

	EMS_MAX UMETA(DyplayName = "DefaultMax")
};



UCLASS()
class C_TEST_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

	/*ENUM**/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	/*COMPONENT **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main | Component", meta = (AllowPrivateAccess = "true"))// spring arm
	class USpringArmComponent* CameraBoom; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main | Component", meta = (AllowPrivateAccess = "true"))// camera
	class UCameraComponent* CameraFollow; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main | Component") //component effetti particellari
	class UParticleSystem* HitParticles;


	/*PLAYER STATS*/
	//TODO NON DEVE ESSERE VISIBILE ANYWHERE
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main | Player Status")
	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Main | Player Stats")
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Main | Player Stats")
	float Stamina;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Main | Player Stats")
	float MaxStamina;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Main | Player Stats")
	float MinStaminaNeededForSprint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Main | Player Stats")
	float StaminaDrainRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Main | Player Stats")
	float RunningSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Main | Player Stats")
	float SprintingSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main | Player Stats")
	int32 GoldCounter;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main | Player Stats")
	bool bInterpToEnemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main | Player Stats")
	bool bMovingForward;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Main | Player Stats")
	//int32 GoldAmount;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Main | Player Stats")
	//int32 HealthAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main | Camera")//valori per gestire la rotazione della camera
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main | Camera")// camera
	float BaseLookUpRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main | Anims")
	bool bIsAttacking;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main | Anims") //permette di settare l'animation Montage
	class UAnimMontage* CombatMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main | Combat")
	bool bHasCombatTarget;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat")
	FVector CombatTargetLocation;
	

	bool bIsShiftKeyPressed;
	bool bIsLeftClickMousePressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main | Anims")
	bool bIsCtrlKeyPressed;
	
	float InterpSpeed;
	/*PLAYER EQUIP*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main | Class Reference")
	class AWeapon* EquippedWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Main | Class Reference") //oggetto che viene settato quando il player entra in collisione con un Item
	class AItem* ActiveOverlappedItem;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main | Class Reference")
	class AEnemy* TargetEnemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Main | Controller") //riferimento a player controller
	class AMainController* MainPlayerController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subclass") // con questo ci assicuriamo che gli oggetti che entrano in collisione col player siano di tipo enemy
	TSubclassOf<AEnemy> EnemyFilter;


	UPROPERTY(EditDefaultsOnly, Category = "Subclass")
	TSubclassOf<class AItemManager> ItemManager; // Riferimento alla classe inventario 

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	void MoveForward(float value);
	void MoveRight(float value);

	/**UPDATE_PLAYER_STATS*/
	void DecrementHealth(float Amount);
	void Die();
	void Attack();
	bool IsDead();
	virtual void Jump() override;


	UFUNCTION(BlueprintCallable) //funzioni che chiamerò dopo l'anim notify delle animation blueprint
	void AttackEnd();	
	UFUNCTION(BlueprintCallable)
	void DeathEnd();
	UFUNCTION(BlueprintCallable)
	void IncrementGold(float Amount);
	UFUNCTION(BlueprintCallable)
	void IncrementHealth(float HealthAmount);
	
	
	/************/
	/*
	@param Rate = -1 a 1 dove 1 corrisponde al 100%
	*/
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void ShiftKeyDown();
	void ShiftKeyUp();
	void LeftClickMouseDown();
	void LeftClickMouseUp();
	void QButtonDown();
	void QButtonUp();
	void CtrlKeyUp();
	void CtrlKeyDown();


	//getter setter
	FORCEINLINE class USpringArmComponent* GetSpringArmComponent() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraFollow; }
	FORCEINLINE EMovementStatus GetMovementStatus() { return MovementStatus; }
	FORCEINLINE void SetStaminaStatus(EStaminaStatus _StaminaStatus) {StaminaStatus = _StaminaStatus;}
	FORCEINLINE void SetActiveOverlappedItem(AItem* _overlappedItem) { ActiveOverlappedItem = _overlappedItem;}
	FORCEINLINE void SetTargetEnemy(AEnemy* _enemy) { TargetEnemy = _enemy;}
	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget; }

	
	void SetInterpToEnemy(bool Interp);
	void SetEquippedWeapon(AWeapon* WeaponToSet);
	void SetMovementStatus(EMovementStatus Status);
	void UpdateCombatTarget();
	FRotator GetLookAtRotationYaw(FVector Target); 

	void ChangeLevel( FName LevelName);

	UFUNCTION(BlueprintCallable)
	void SaveGame();
	UFUNCTION(BlueprintCallable)
	void LoadGame(bool setLocation);

	


};
