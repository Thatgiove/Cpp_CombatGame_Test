#include "MainController.h"
#include "Blueprint/UserWidget.h"







void AMainController::BeginPlay()
{
	Super::BeginPlay();


	bPauseDisplayVisible = false;

	//se è presente un elementop Widget selezionato nel menu a tendina
	if (UserOverlayAsset)
	{
		HUD_Overlay = CreateWidget<UUserWidget>(this, UserOverlayAsset);
	}
	HUD_Overlay->AddToViewport();
	HUD_Overlay->SetVisibility(ESlateVisibility::Visible);


	//se è presente un elem widget lo renderizzo
	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
		FVector2D Alignment(0.f, 0.f);
		EnemyHealthBar->SetAlignmentInViewport(Alignment);
	}


	if (WPauseMenu)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);

		if (PauseMenu)
		{
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);

		}	
	}


}


void AMainController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (EnemyHealthBar)
	{
		FVector2D ViewportPosition;
		ProjectWorldLocationToScreen(EnemyLocation, ViewportPosition);
		ViewportPosition.Y -= 85.f;

		FVector2D SizeInViewport(300.f, 25.f);

		EnemyHealthBar->SetPositionInViewport(ViewportPosition);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);

	}








}



void AMainController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}
void AMainController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainController::DisplayPauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseDisplayVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI InputModeGameAndUI;

		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;
	}
}

void AMainController::RemovePauseMenu_Implementation()
{
	if (PauseMenu)
	{
		//GameModeOnly();

		bShowMouseCursor = false;

		bPauseDisplayVisible = false;
	}
}


void AMainController::TogglePauseMenu()
{
	bPauseDisplayVisible = !bPauseDisplayVisible;
	
	if (bPauseDisplayVisible)
	{
		PauseMenu->SetVisibility(ESlateVisibility::Visible);
	}else
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);
}


