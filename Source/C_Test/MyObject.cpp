// Fill out your copyright notice in the Description page of Project Settings.


#include "MyObject.h"

UMyObject::UMyObject() //vado a definire gli elementi defininiti nel file .h --> in questo caso il costruttore
{
	myfloat = 0;
}

void UMyObject::Myfunction() //vado a definire gli elementi defininiti nel file .h --> in questo caso il costruttore
{
	UE_LOG(LogTemp, Warning, TEXT("Test di Warning"));
}