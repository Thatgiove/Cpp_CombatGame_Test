#include "Enemy.h"
#include "MainCharacter.h"
#include "AIController.h"
#include "MainController.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "Engine/SkeletalMeshSocket.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(900.f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(150.f);

	ArmCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ArmCollision"));
	ArmCollision->SetupAttachment(GetMesh(), FName("EnemySocket")); //attacco la box collision al socket che ho creato nella skeletalMesh -- ha il nome comune a tutti i nemici


	bAttacking = false;
	bInterpToCharacter = false;
	InterpSpeed = 15.f;
	bOverlappingCombatSphere = false;
	bHasValidTarget = false;
	Health = 50;
	MaxHealth = 100;
	AttackMinTime = 0.2f;
	AttackMaxTime = 0.9f;
	Damage = 11;
	EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//castiamo ai controller 
	AIController = Cast<AAIController>(GetController());
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);
	
	ArmCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::ArmCollisionOverlapBegin);
	ArmCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::ArmCollisionOverlapEnd);


	ArmCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ArmCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	ArmCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ArmCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);





	//funzione di interpolazione
	if (CombatTarget && bInterpToCharacter && Alive())
	{
		//con questa funzione di interpolazione 
		//facciamo ruotare la capsula del player in 
		// direzione del nemico 
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotation);
	}

}

FRotator AEnemy::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

void AEnemy::SetInterpToEnemy(bool Interp)
{
	bInterpToCharacter = Interp;
}



void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}




//quando si ha un collisione con la sfera del raggio visivo
void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{

		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main)
		{
			SetHasValidTarget(true);
			MoveToTarget(Main);

		}

		
	}
}
void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	if (OtherActor && Alive())
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main)
		{
			/*if (Main->MainPlayerController)
			{*/
				Main->SetHasCombatTarget(false);

				if (Main->TargetEnemy == this)
				{
					Main->SetTargetEnemy(nullptr);
				}

				Main->UpdateCombatTarget();
				//Main->MainPlayerController->RemoveEnemyHealthBar(); //chiamiamo il controller del character per nascondere il widget

			//}
		}


	}
	
	/*if (AIController)
	{
		AIController->StopMovement();
	}*/
}






//collisione con la sfera di combattimento
void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	
	if (OtherActor && Alive())
	{
		bOverlappingCombatSphere = true;
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main)
		{
			Main->SetTargetEnemy(this); //questa istanza dell'enemy diventa il target del Main
			
			Main->SetHasCombatTarget(true);

			Main->UpdateCombatTarget();
			//if (Main->MainPlayerController)
			//{
			//	Main->SetHasCombatTarget(true);
			//	Main->MainPlayerController->DisplayEnemyHealthBar(); //chiamiamo il controller del character per mostrare il widget

			//}

			CombatTarget = Main;
			float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
			//Attack();
			
		}
	}
}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && Alive())
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main)
		{
			
			if (Main->TargetEnemy == this)
			{
				Main->SetTargetEnemy(nullptr);
				Main->bHasCombatTarget = false;
				Main->UpdateCombatTarget();
			}
	/*		if (Main->MainPlayerController)
			{
				USkeletalMeshComponent* MainMesh = Cast<USkeletalMeshComponent>(OtherComp);
				if (MainMesh) Main->MainPlayerController->RemoveEnemyHealthBar();
			}*/
			bOverlappingCombatSphere = false; //accoppiata con la fine dell'animazione dell'attacco
			MoveToTarget(Main);


			GetWorldTimerManager().ClearTimer(AttackTimer);

		}

		
	}
}




//collisioni con il braccio del nemico
//sul player al momento dell'attacco
void AEnemy::ArmCollisionOverlapBegin
(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult
)
{
	if (OtherActor && Alive())
	{
		UE_LOG(LogTemp, Warning, TEXT("ArmCollision!!!!!!!!!!!!!! "));
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main)
		{
			if (Main->HitParticles)
			{
				//spawn dell'effetto particellare del player nella location del socket
				const USkeletalMeshSocket* ParticleSocket = GetMesh()->GetSocketByName("TipSocket");
				if (ParticleSocket)
				{
					FVector SocketLocation = ParticleSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Main->HitParticles, SocketLocation, FRotator(0.f), false);
				}

			}

			//applica danno del tipo DamageTypeClass da definire nella blueprint
			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Main, Damage, AIController, this, DamageTypeClass);
			}
		}
	}

}

void AEnemy::ArmCollisionOverlapEnd
(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex
)
{


}

//Attivo e disattivo le collisioni del braccio in base 
//all'animazione definita nell'animation blueprint
void AEnemy::ActivateArmCollision()
{
	ArmCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (SwingSound_sfx)
	{
		UGameplayStatics::PlaySound2D(this, SwingSound_sfx);
	}
}


void AEnemy::DeactivateArmCollision()
{
	ArmCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



void AEnemy::MoveToTarget(AMainCharacter* Target)
{

	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(25.0f);

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);
	}
}
void AEnemy::Attack()
{
	if (Alive() && bHasValidTarget)
	{
		if (AIController)
		{
			AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
		if (!bAttacking)
		{
			bAttacking = true;
			SetInterpToEnemy(true);



			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && CombatMontage)
			{
				AnimInstance->Montage_Play(CombatMontage, 1.35f);
				AnimInstance->Montage_JumpToSection(FName("Attack_C"), CombatMontage);
			}
		}
	}
	
	
}

void AEnemy::AttackEnd()
{
	bAttacking = false;
	SetInterpToEnemy(false);

	if (bOverlappingCombatSphere)
	{
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
	}

}

void AEnemy::DeathEnd()
{
	bOverlappingCombatSphere = false;
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	//GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay);
}




float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

		if (Health - DamageAmount <= 0.f)
		{
			Health = 0.f;
			Die(DamageCauser);
		}
		else
		{
			Health -= DamageAmount;
		}
		return DamageAmount;

}

void AEnemy::Die(AActor* Causer)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.35f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
	}
	ArmCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bAttacking = false;

	AMainCharacter* Main = Cast<AMainCharacter>(Causer);
	if (Main)
	{
		Main->UpdateCombatTarget();
	}


}


bool AEnemy::Alive()
{
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}




//void AEnemy::Disappear()
//{
//	Destroy();
//}