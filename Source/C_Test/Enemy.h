#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus :uint8
{
	EMS_Idle			UMETA(DeplayName = "Idle"),
	EMS_MoveToTarget	UMETA(DeplayName = "MoveToTarget"),
	EMS_Attacking		UMETA(DeplayName = "Attacking"),
	EMS_Dead			UMETA(DeplayName = "Dead"),

	EMS_MAX				UMETA(DeplayName = "DefaulMAX")
};

UCLASS()
class C_TEST_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	/****COMPONENTI****/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Enum")//enum per lo stato e le animazioni
	EEnemyMovementStatus EnemyMovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Component") //sfera di collisione che definisce il raggio visivo del nemico 
	class USphereComponent* AgroSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Component") //sfera di collisione che definisce il raggio di combattimento del nemico 
	USphereComponent* CombatSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy | Component")//collisione braccio per danni al player
	class UBoxComponent* ArmCollision;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Particles")//component effetti particellari
	class UParticleSystem* HitParticles;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Combat")//controller per il movimento e azioni
	class AAIController* AIController;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy | Combat") //riferimento a main character per target
	AMainCharacter* CombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Anims")//riferimento al montage delle animazioni
	class UAnimMontage* CombatMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Sound")
	class USoundCue* HitSound_sfx;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Sound")
	USoundCue* SwingSound_sfx;



	/***PROPRIETA*****/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Property")
	float Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Property")
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Property")
	float Damage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Property")
	bool bAttacking;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy | Property")
	bool bOverlappingCombatSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Property")
	bool bInterpToCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy | Property")
	bool bHasValidTarget;
	float InterpSpeed;

	FTimerHandle AttackTimer; //timer che va a settare il tempo tra un attacco e l'altro 
	float AttackMinTime;
	float AttackMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Property") //proprietà per il ApplyDamage -- serve a definire il tipo di danno -- da settare in blueprint
	TSubclassOf<UDamageType> DamageTypeClass;




	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	//le funzioni che vanno overlapbegin e overlapEnd delle collisioni
	UFUNCTION()
	void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void ArmCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void ArmCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	//chiamate nell'event graph dell'animation blueprint
	UFUNCTION(BlueprintCallable)
	void ActivateArmCollision();
	UFUNCTION(BlueprintCallable)
	void DeactivateArmCollision();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	void Die(AActor* Causer);
	bool Alive();

	//la chiamiamo dalla blueprint perché abbiamo bisogno che prima finisca l'aniamzione
	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMainCharacter* Target);
	void Attack();

	FRotator GetLookAtRotationYaw(FVector Target);



	//getter setter 
	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus _EnemyMovementStatus) { EnemyMovementStatus = _EnemyMovementStatus; }
	FORCEINLINE void SetHasValidTarget(bool _isValidTarget) { bHasValidTarget = _isValidTarget; }
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() { return EnemyMovementStatus; }

	void SetInterpToEnemy(bool Interp);
};
