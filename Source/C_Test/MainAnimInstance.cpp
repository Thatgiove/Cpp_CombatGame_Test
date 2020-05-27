#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharacter.h"



//inizializza
void UMainAnimInstance::NativeInitializeAnimation() 
{
	//se il pawn � null
	if (CharacterPawn == nullptr)
	{
		//proviamo a prenderlo da chi posside questa AnimInstance
		CharacterPawn = TryGetPawnOwner();
		
		// se il pawn � valido castiamo il mainCharacter 
		//cos� nell'animation blueprint possiamo usare le sue propriet�
		if(CharacterPawn)
		MainCharacter = Cast<AMainCharacter>(CharacterPawn);



	}
}

void UMainAnimInstance::UpdateAnimationProperty()
{
	if (CharacterPawn == nullptr)
	{
		//proviamo a prenderlo da chi posside questa AnimInstance
		CharacterPawn = TryGetPawnOwner();
	}
	if (CharacterPawn)
	{
		
		//a che velocit� sta andando il pawn a quel frame
		FVector Speed = CharacterPawn->GetVelocity(); 
		FVector LateralSpeed = FVector(Speed.X , Speed.Y , 0.f);

		MovementSpeed = LateralSpeed.Size(); //Size() ottiene la magnitudine del vettore
		
		bIsInAir = CharacterPawn->GetMovementComponent()->IsFalling();

		//bCanRoll = CharacterPawn->GetMovementComponent()->IsCrouching() ;
		
		
		
		AMainCharacter* main = Cast<AMainCharacter>(CharacterPawn);
		//bIsJumpKeyPressed = MainCharacter->bPressedJump;

		if (MainCharacter == nullptr)
		{
			MainCharacter = Cast<AMainCharacter>(CharacterPawn);

			bCanRoll = MainCharacter->bMovingForward && MainCharacter->bIsCtrlKeyPressed;

		}
	}
}