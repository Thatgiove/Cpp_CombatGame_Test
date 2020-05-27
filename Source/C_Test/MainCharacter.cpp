#include "MainCharacter.h"
#include "Engine.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"

#include "ItemManager.h"
#include "Weapon.h"
#include "Item.h"
#include "Enemy.h"
#include "MainController.h"
#include "MySaveGame.h"



// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//springArm + proprietà
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetRelativeRotation(FRotator(-45.f, 0, 0));
	CameraBoom->TargetArmLength = 400; // lunghezza del braccio 
	CameraBoom->bEnableCameraLag = true; // camera segue con leggero ritardo
	CameraBoom->CameraLagSpeed = 3.f;
	CameraBoom->bUsePawnControlRotation = true; // utilizza la rotation del pawn controller;

	//camera
	CameraFollow = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraFollow->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); //attravero il socket posso attaccare oggetti al component
	//attacco la camera alla fine dello string arm 
	//e lascio gestire ad essa la rotazione
	CameraFollow->bUsePawnControlRotation = false;


	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	//il charater non ruota
	//quando ruota il controller
	//ruota solo la camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//in questo modo la direzione forward  del player definita nel characterMovement 
	// è la stessa della camera
	//orienta la rotazione al movimento
	GetCharacterMovement()->bOrientRotationToMovement = true; //il characterMovement segue la direzione dell'input
	GetCharacterMovement()->RotationRate = FRotator(0, 500.f, 0); //alla velocità 500 sull'asse z -- yaw

	GetCharacterMovement()->JumpZVelocity = 550.f; // forza del salto
	GetCharacterMovement()->AirControl = 0.2f; //a che velocita si muove mentre è in aria

	//set default enums
	MovementStatus = EMovementStatus::EMS_NORMAL;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	Health = 50;
	MaxHealth = 100;
	Stamina = 80;
	MaxStamina = 100;
	
	MinStaminaNeededForSprint = 40;
	StaminaDrainRate = 34.f;


	RunningSpeed = 650.f ;
	SprintingSpeed = 950.f;

	GoldCounter = 1;
	//GoldAmount = 100;
	//HealthAmount = 10;


	bIsShiftKeyPressed = false;
	bMovingForward = false;
	bIsLeftClickMousePressed = false;

	
	
	InterpSpeed = 15.f;
	bInterpToEnemy = false;
	bHasCombatTarget = false;
	bIsCtrlKeyPressed = false;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	MainPlayerController = Cast<AMainController>(GetController()); //get del player controller



}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead()) return;


	float DeltaStamina = StaminaDrainRate * DeltaTime;

	//se è presente un target enemy calcolo la sua posizione ogni tick e la passo al controller 
	//che così sa dove renderizzare il widget
	if (TargetEnemy)
	{
		CombatTargetLocation = TargetEnemy->GetActorLocation();
		if (MainPlayerController)
		{
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
	//funzione di interpolazione
	//funzione di interpolazione
	// TODO --- bInterpToEnemy viene settato a true quando colpisco con arma
	// TODO --- hasCombatTarger non viene resettato quando esco dalla collisione
	if (TargetEnemy && bInterpToEnemy && !IsDead())
	{
		//con questa funzione di interpolazione 
		//facciamo ruotare la capsula del player in 
		// direzione del nemico 
		FRotator LookAtYaw = GetLookAtRotationYaw(TargetEnemy->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotation);


	}



	//switch (StaminaStatus)
	//{
	//case EStaminaStatus::ESS_Normal:
	//	if (bIsShiftKeyPressed)
	//	{
	//		if (Stamina - DeltaStamina <= MinSprintStamina)
	//		{
	//			SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
	//			Stamina -= DeltaStamina;
	//		}
	//		else
	//		{
	//			Stamina -= DeltaStamina;
	//		}
	//		if (bMovingForward || bMovingRight)
	//		{
	//			SetMovementStatus(EMovementStatus::EMS_Sprinting);
	//		}
	//		else
	//		{
	//			SetMovementStatus(EMovementStatus::EMS_Normal);
	//		}
	//	}
	//	else // Shift key up
	//	{
	//		if (Stamina + DeltaStamina >= MaxStamina)
	//		{
	//			Stamina = MaxStamina;
	//		}
	//		else
	//		{
	//			Stamina += DeltaStamina;
	//		}
	//		SetMovementStatus(EMovementStatus::EMS_Normal);
	//	}
	//	break;
	//case EStaminaStatus::ESS_BelowMinimum:
	//	if (bShiftKeyDown)
	//	{
	//		if (Stamina - DeltaStamina <= 0.f)
	//		{
	//			SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
	//			Stamina = 0;
	//			SetMovementStatus(EMovementStatus::EMS_Normal);
	//		}
	//		else
	//		{
	//			Stamina -= DeltaStamina;
	//			if (bMovingForward || bMovingRight)
	//			{
	//				SetMovementStatus(EMovementStatus::EMS_Sprinting);
	//			}
	//			else
	//			{
	//				SetMovementStatus(EMovementStatus::EMS_Normal);
	//			}
	//		}
	//	}
	//	else // Shift key up
	//	{
	//		if (Stamina + DeltaStamina >= MinSprintStamina)
	//		{
	//			SetStaminaStatus(EStaminaStatus::ESS_Normal);
	//			Stamina += DeltaStamina;
	//		}
	//		else
	//		{
	//			Stamina += DeltaStamina;
	//		}
	//		SetMovementStatus(EMovementStatus::EMS_Normal);
	//	}
	//	break;
	//case EStaminaStatus::ESS_Exhausted:
	//	if (bShiftKeyDown)
	//	{
	//		Stamina = 0.f;
	//	}
	//	else // Shift key up
	//	{
	//		SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
	//		Stamina += DeltaStamina;
	//	}
	//	SetMovementStatus(EMovementStatus::EMS_Normal);
	//	break;
	//case EStaminaStatus::ESS_ExhaustedRecovering:
	//	if (Stamina + DeltaStamina >= MinSprintStamina)
	//	{
	//		SetStaminaStatus(EStaminaStatus::ESS_Normal);
	//		Stamina += DeltaStamina;
	//	}
	//	else
	//	{
	//		Stamina += DeltaStamina;
	//	}
	//	SetMovementStatus(EMovementStatus::EMS_Normal);
	//	break;
	//default:
	//	;
	//}

	//TODO -- BLOCCARE Sprint quando il character non si muove o sta cadendo
	if (bIsShiftKeyPressed && 
		bMovingForward && 
		StaminaStatus != EStaminaStatus::ESS_Exhausted_Recovering
		
		)
	{
		SetMovementStatus(EMovementStatus::EMS_SPRINTING);
		Stamina -= DeltaStamina;

		if (Stamina <= MinStaminaNeededForSprint)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Below_Minum);
		}

		if (Stamina <= 0)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
			SetMovementStatus(EMovementStatus::EMS_NORMAL);
			Stamina = 0; // smetto di sottrarre la stamina
		}
	}
		
	else 
	{
		SetMovementStatus(EMovementStatus::EMS_NORMAL);
		SetStaminaStatus(EStaminaStatus::ESS_Exhausted_Recovering);


		if (Stamina <= MaxStamina)
			Stamina += DeltaStamina;
		
		if(Stamina > MinStaminaNeededForSprint)
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
		
	}
		
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Stamina: Stamina: %f"), Stamina));

}



FRotator AMainCharacter::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

void AMainCharacter::ChangeLevel(FName LevelName)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevelName = World->GetMapName(); //get nome livello

		if ((*CurrentLevelName) != LevelName) //vado a leggere il contenuto dell'indirizzo 
		{
			UGameplayStatics::OpenLevel(World, LevelName);
		}

	}



}




void AMainCharacter::SaveGame()
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	SaveGameInstance->CharacterStats.Health = Health;
	SaveGameInstance->CharacterStats.MaxHealth = MaxHealth;
	SaveGameInstance->CharacterStats.Stamina = Stamina;
	SaveGameInstance->CharacterStats.MaxStamina = MaxStamina;
	SaveGameInstance->CharacterStats.GoldCounter = GoldCounter;

	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

	SaveGameInstance->CharacterStats.LevelName = MapName;

	if (EquippedWeapon) //salva la weapon solo se è equipaggiata
	{
		SaveGameInstance->CharacterStats.WeaponName = EquippedWeapon->Name;
	}

	SaveGameInstance->CharacterStats.Location = GetActorLocation();
	SaveGameInstance->CharacterStats.Rotation = GetActorRotation();

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SlotName, SaveGameInstance->SlotIndex);





}
void AMainCharacter::LoadGame(bool setPosition)
{
	UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SlotName, LoadGameInstance->SlotIndex));
	
	
	
	
	Health = LoadGameInstance->CharacterStats.Health;
	MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
	Stamina = LoadGameInstance->CharacterStats.Stamina;
	MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;
	GoldCounter = LoadGameInstance->CharacterStats.GoldCounter;

	
	if (ItemManager) //se presente l'itemManager (inventario)
	{
		AItemManager* Weapons = GetWorld()->SpawnActor<AItemManager>(ItemManager); //casto la classe
		if (Weapons) // se il cast ha avuto buon fine recupero l'oggetto che mi interessa dalla mappa
		{
			FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;
			//TODO se la string è empty va in crash nullptr!!!
			AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponList[WeaponName]);
			WeaponToEquip->Equip(this);

		}
	}

	//TODO la position e la rotation vengono settate solo se non si cambia livello

	//if (setPosition)
	//{
		SetActorLocation(LoadGameInstance->CharacterStats.Location);
		SetActorRotation(LoadGameInstance->CharacterStats.Rotation);
	/*}*/

	//SetMovementStatus(EMovementStatus::EMS_Normal);
	//GetMesh()->bPauseAnims = false;
	//GetMesh()->bNoSkeletonUpdate = false;

	//if (LoadGameInstance->CharacterStats.LevelName != TEXT(""))
	//{
	//	FName LevelName(*LoadGameInstance->CharacterStats.LevelName);

	//	SwitchLevel(LevelName);
	//}


}





// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//controlla che sia valido
	check(PlayerInputComponent);

	//mappo un'azione singola
	//jump
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMainCharacter::Jump); //alla pressione del tasto -- override del jump perché devo prima controllere che non sia dead
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping); //al rilascio del tasto

	//sprint
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AMainCharacter::ShiftKeyDown); //alla pressione del tasto
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AMainCharacter::ShiftKeyUp); //al rilascio del tasto

	//roll
	PlayerInputComponent->BindAction(TEXT("Roll"), IE_Pressed, this, &AMainCharacter::CtrlKeyDown); //alla pressione del tasto
	PlayerInputComponent->BindAction(TEXT("Roll"), IE_Released, this, &AMainCharacter::CtrlKeyUp); //al rilascio del tasto

	//click menu pausa
	PlayerInputComponent->BindAction(TEXT("Pause"), IE_Pressed, this, &AMainCharacter::QButtonDown); //alla pressione del tasto
	PlayerInputComponent->BindAction(TEXT("Pause"), IE_Released, this, &AMainCharacter::QButtonUp); //al rilascio del tasto

	//click tastosx
	PlayerInputComponent->BindAction(TEXT("LeftMouseButton"), IE_Pressed, this, &AMainCharacter::LeftClickMouseDown); //alla pressione del tasto
	PlayerInputComponent->BindAction(TEXT("LeftMouseButton"), IE_Released, this, &AMainCharacter::LeftClickMouseUp); //al rilascio del tasto



	//move
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCharacter::MoveRight);

	//passo l'indirizzo della funzione AddControllerYawInput del APawn
	// per effettuare una rotazione col mouse
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &APawn::AddControllerPitchInput);

	//per effettuare una rotazione con i tasti direzionali
	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AMainCharacter::LookUpAtRate);

}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
	if (Health - DamageAmount <= 0.f)
	{
		Health -= DamageAmount;
		Die();
		if (DamageCauser)
		{
			// il nemico non ha più un target valido
			// così può ritornare a girovagare
			AEnemy* Enemy = Cast<AEnemy>(DamageCauser);
			if (Enemy)
			{
				Enemy->SetHasValidTarget(false);
			}
		}
	}
	else
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}

void AMainCharacter::MoveForward(float value)
{
	if (value > 0) bMovingForward = true; else bMovingForward = false;

	//vado a controllare che ci sia un player controller e che il suo valore sia != 0
	//il controller è ereditato da Pawn
	if (Controller != nullptr && value != 0 && !bIsAttacking && !IsDead())
	{
		//troviamo la direzione forward in base alla rotation del controller
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0); //nuovo rotator basato sulla rotazione del controller

		//calcolka il valore di x dalla matrice della rotazione
		//in questo modo indipendentemente dalla rotazione sullo Yaw
		//abbiamo il forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, value);
	}
}
void AMainCharacter::MoveRight(float value )
{
	//vado a controllare che ci sia un player controller e che il suo valore sia != 0
	//il controller è ereditato da Pawn
	if (Controller != nullptr && value != 0 && !bIsAttacking && !IsDead())
	{
		//troviamo la direzione forward in base alla rotation del controller
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0); //nuovo rotator basato sulla rotazione del controller

		//calcolka il valore di x dalla matrice della rotazione
		//in questo modo indipendentemente dalla rotazione sullo Yaw
		//abbiamo il forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, value);
	}
}
void AMainCharacter::DecrementHealth(float Amount)
{
	Health -= Amount;

	if (Health <= 0)
		Die();
}
void AMainCharacter::IncrementGold(float Amount)
{
	GoldCounter += Amount;
}
void AMainCharacter::IncrementHealth(float HealthAmount)
{
	if (Health + HealthAmount >= MaxHealth)
	{
		Health = MaxHealth;
	}
	else
	{
		Health += HealthAmount;
	}
}


void AMainCharacter::Die()
{
	//UE_LOG(LogTemp, Warning, TEXT("SEI MORTO!"));
	SetMovementStatus(EMovementStatus::EMS_DEATH);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.35f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainCharacter::Attack()
{
	if (!bIsAttacking)
	{
		bIsAttacking = true;
		SetInterpToEnemy(true);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			
			AnimInstance->Montage_Play(CombatMontage, 2.2f);
			AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
			
			//scegli randomicamente l'attacco da effettuare
			/*int32 Section = FMath::RandRange(0, 2);
			switch (Section)
			{
			case 0:
				AnimInstance->Montage_Play(CombatMontage, 2.2f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
				break;
			case 1:
				AnimInstance->Montage_Play(CombatMontage, 1.8f);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
				break;
			case 2:
				AnimInstance->Montage_Play(CombatMontage, 1.8f);
				AnimInstance->Montage_JumpToSection(FName("Attack_3"), CombatMontage);
				break;
			default:
				;
			}*/
		}



	}
	

}

bool AMainCharacter::IsDead()
{
	return GetMovementStatus() == EMovementStatus::EMS_DEATH;
}


void AMainCharacter::Jump()
{
	if (!IsDead())
		Super::Jump();

}
void AMainCharacter::AttackEnd()
{
		bIsAttacking = false ;
		SetInterpToEnemy(false);
}

void AMainCharacter::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}


void AMainCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() /*prendo il deltaTime fuori dal Tick*/);
}
void AMainCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() /*prendo il deltaTime fuori dal Tick*/);

}


void AMainCharacter::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

void AMainCharacter::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	EquippedWeapon = WeaponToSet;
}

void AMainCharacter::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (Status == EMovementStatus::EMS_SPRINTING)
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	else
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
}




void AMainCharacter::ShiftKeyDown() { bIsShiftKeyPressed = true; }
void AMainCharacter::ShiftKeyUp() { bIsShiftKeyPressed = false; }
void AMainCharacter::QButtonDown() { if(MainPlayerController) MainPlayerController->TogglePauseMenu();}
void AMainCharacter::QButtonUp() { /*if (MainPlayerController) MainPlayerController->TogglePauseMenu(false);*/ }


void AMainCharacter::CtrlKeyDown(){ bIsCtrlKeyPressed = true; }
void AMainCharacter::CtrlKeyUp(){ bIsCtrlKeyPressed = false; }



void AMainCharacter::LeftClickMouseDown() 
{ 
	
	//se overlap con oggetto TODO -- deve essere di tipo arma
	if (ActiveOverlappedItem)
	{
		//cast weapon 
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappedItem);
		//TODO -- non deve equipaggiare ad ogni click
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappedItem(nullptr);
		}
	}
	//se ho equipaggiato oggetto di tipo arma
	//e non sto gia attaccando
	else if (EquippedWeapon && !bIsAttacking )
	{
		Attack();
	}
	bIsLeftClickMousePressed = true;


}
void AMainCharacter::LeftClickMouseUp() 
{ 
	bIsLeftClickMousePressed = false; 
}


// ci assicuriamo che il nemico più vicino sia il target del player
void AMainCharacter::UpdateCombatTarget()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter); //prende la lista degli attori di tipo EnemyFilter che stanno in collisione

	if (OverlappingActors.Num() == 0)
	{
		if (MainPlayerController)
		{
			MainPlayerController->RemoveEnemyHealthBar();
		}
		return;
	}

	AEnemy* ClosestEnemy = Cast<AEnemy>(OverlappingActors[0]);
	if (ClosestEnemy)
	{
		FVector Location = GetActorLocation();
		float MinDistance = (ClosestEnemy->GetActorLocation() - Location).Size();

		for (auto Actor : OverlappingActors)
		{
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy)
			{
				float DistanceToActor = (Enemy->GetActorLocation() - Location).Size();
				if (DistanceToActor < MinDistance)
				{
					MinDistance = DistanceToActor;
					ClosestEnemy = Enemy;
				}
			}
		}
		if (MainPlayerController)
		{
			MainPlayerController->DisplayEnemyHealthBar();
		}
		SetTargetEnemy(ClosestEnemy);
		bHasCombatTarget = true;
	}


}