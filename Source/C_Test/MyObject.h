// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyObject.generated.h"

/**
 *
 */
UCLASS(Blueprintable) // ci permette di renderla blueprint
class C_TEST_API UMyObject : public UObject
{
	GENERATED_BODY()

		

public:
	UMyObject(); // costruttore

	UPROPERTY(BlueprintReadWrite, Category = "MieVariabili")  // attributo che ci permette di editarla nell'editor
		float myfloat;

	UFUNCTION(BlueprintCallable, Category = "MieFunzioni")
		void Myfunction();

};
