// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class C_TEST_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		bool bIsInAir;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		bool bIsJumpKeyPressed;

	//riferimento al characterPawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* CharacterPawn;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class AMainCharacter* MainCharacter;

	//override di un metodo che inizializza l'animazione, simile ad un beginplay
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	//funzione simile al tick che fa l'update delle varibili
	void UpdateAnimationProperty();





};
