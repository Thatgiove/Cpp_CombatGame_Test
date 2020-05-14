#include "Weapon.h"
#include "MainCharacter.h"
#include "Enemy.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"

#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


AWeapon::AWeapon()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh "));
	SkeletalMeshComponent->SetupAttachment(GetRootComponent());

	BoxCombatCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Combat Collider "));
	BoxCombatCollider->SetupAttachment(GetRootComponent());

	
	Damage = 25.f;

	WeaponStatus = EWeaponStatus::EWS_PICKUP;


}
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	//inizializza collider e setta proprieta del box collider
	BoxCombatCollider->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnCombatOverlapBegin);
	BoxCombatCollider->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnCombatOverlapEnd);

	BoxCombatCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCombatCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxCombatCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCombatCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AWeapon::Equip(AMainCharacter* MainCharacter)
{
	if (MainCharacter)
	{
		SetWeaponOwner(MainCharacter->GetController());

		SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		SkeletalMeshComponent->SetSimulatePhysics(false);

		//prendo il socket che ho creato in unreal 
		const USkeletalMeshSocket* RightHandSocket = MainCharacter->GetMesh()->GetSocketByName("RightHandSocket");
		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(this, MainCharacter->GetMesh());
			//bRotate = false;

			MainCharacter->SetEquippedWeapon(this);
			//MainCharacter->SetActiveOverlappingItem(nullptr);
		}

		if (WeaponPickUp_SFX)
		{
			UGameplayStatics::PlaySound2D(this, WeaponPickUp_SFX);
		}

		SetWeaponStatus(EWeaponStatus::EWS_EQUIPPED);
	}
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor && WeaponStatus == EWeaponStatus::EWS_PICKUP)
	{
		//cast del main
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
		if (MainCharacter)
		{
			//setta l'oggetto della classe corrente come oggetto equipaggiato
			MainCharacter->SetActiveOverlappedItem(this);
			//Equip(MainCharacter);
		}


	}


}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor)
	{
		//cast del main
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);

		if (MainCharacter)
		{
			MainCharacter->SetActiveOverlappedItem(nullptr);
		}

		

	}
	
	
}

void AWeapon::OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) 
	{
		AEnemy* Enemy = Cast< AEnemy>(OtherActor);
		if (Enemy)
		{
			if (Enemy->HitParticles)
			{
				//recupera il socket della skeletal mesh 
				const USkeletalMeshSocket* WeaponSocket = SkeletalMeshComponent->GetSocketByName("socketForParticle");
				if (WeaponSocket)
				{
					//se esiste spawn l'emitter nella location del socket
					FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMeshComponent);

					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticles, SocketLocation, FRotator(0.f), false);
				}
			}
			if (Enemy->HitSound_sfx)
			{
				UGameplayStatics::PlaySound2D(this, Enemy->HitSound_sfx);
			}
			if (DamageTypeClass) //se definisco il typeClass nella blueprint posso assegnare il danno passo come controller del player
			{
				UGameplayStatics::ApplyDamage(Enemy, Damage, WeaponOwner, this, DamageTypeClass);
			}
		}


	}



}

void AWeapon::OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
void AWeapon::ActivateCollision()
{
	BoxCombatCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


void AWeapon::DeactivateCollision()
{
	BoxCombatCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::PlaySoundSwing()
{
	if (Swing_SFX)
	{
		UGameplayStatics::PlaySound2D(this, Swing_SFX);
	}
}

