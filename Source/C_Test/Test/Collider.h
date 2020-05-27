// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Collider.generated.h"

UCLASS()
class C_TEST_API ACollider : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "Collider Pawn")
	class USphereComponent* SphereComponent; // per le collision

	UPROPERTY(VisibleAnywhere, Category = "Collider Pawn")
	class UStaticMeshComponent* StaticMeshComponent;// per le collision
	
	UPROPERTY(VisibleAnywhere, Category = "Collider Pawn")
	class UCameraComponent* CameraComponent; // camera
	
	UPROPERTY(VisibleAnywhere, Category = "Collider Pawn")
	class USpringArmComponent* SpringArmComponent; // spring arm
	
	UPROPERTY(VisibleAnywhere, Category = "Collider Pawn")
	class UColliderMovementComponent* OurMovementComponent; // riferimento al movement componente creatp

	virtual UPawnMovementComponent* GetMovementComponent() const override; //movement component della classe base


	//GETTER SETTER
	FORCEINLINE  USphereComponent* GetSphereComponent() { return SphereComponent; };
	FORCEINLINE  void SetSphereComponent(USphereComponent* Sphere) { SphereComponent = Sphere; };

	FORCEINLINE  UStaticMeshComponent* GetStaticMeshComponent() { return StaticMeshComponent; };
	FORCEINLINE  void SetStaticMeshComponent(UStaticMeshComponent* Mesh) { StaticMeshComponent = Mesh; };

	FORCEINLINE  UCameraComponent* GetCameraComponent() { return CameraComponent; };
	FORCEINLINE  void SetUCameraComponent(UCameraComponent* Camera) { CameraComponent = Camera; };

	FORCEINLINE  USpringArmComponent* GetSpringArmComponent() { return SpringArmComponent; };
	FORCEINLINE  void SetSpringArmComponent(USpringArmComponent* SpringArm) { SpringArmComponent = SpringArm; };

private:
//	UStaticMeshComponent* StaticMeshComponent;
	//funzioni movimento
	void MoveForward(float input);
	void MoveRight(float input);


	void CameraYaw(float value);
	void CameraPitch(float value);

	//vettore 2d che serve per la rotazione
	FVector2D CameraRotation;
	FVector Velocity;
	float MaxSpeed;
};
