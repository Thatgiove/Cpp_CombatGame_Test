#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"


UENUM(BlueprintType)
enum class EWeaponStatus : uint8
{
	EWS_PICKUP  	UMETA(DisplayName = "Pickup"),
	EWS_EQUIPPED     UMETA(DyplayName = "Equipped"),

	EMS_MAX UMETA(DyplayName = "DefaultMax")
};



UCLASS()
class C_TEST_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	class USkeletalMeshComponent* SkeletalMeshComponent;
protected:
	virtual void BeginPlay() override;

public :
	//effetto sonoro quando prende arma
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Sound")
	class USoundCue* WeaponPickUp_SFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Sound")
	class USoundCue* Swing_SFX;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon | Weapon Status")
	EWeaponStatus WeaponStatus;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | Combat")
	class UBoxComponent* BoxCombatCollider;
	
	
	/***PROPRIETA ARMA*****/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon | Property")
	FString Name;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | Property")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Property") //proprietà per ApplyDamage -- serve a definire il tipo di danno -- da settare in blueprint
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | Property")
	AController* WeaponOwner;

	FORCEINLINE void SetWeaponOwner(AController* Inst) { WeaponOwner = Inst; }
	
	
	
	
	
	
	
	void Equip(class AMainCharacter* MainCharacter);



	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	UFUNCTION()
	void OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//setter getter
	FORCEINLINE void SetWeaponStatus(EWeaponStatus _WeaponStatus) { WeaponStatus= _WeaponStatus; }




	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

	UFUNCTION(BlueprintCallable)
	void PlaySoundSwing();
};
