
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainController.generated.h"

/**
 * 
 */
UCLASS()
class C_TEST_API AMainController : public APlayerController
{
	GENERATED_BODY()
public: 

    //creiamo un menu a tendina per poter selezionare il nostro widget
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
    TSubclassOf<class UUserWidget> UserOverlayAsset;
    //quello che utilizzerò in game
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
    UUserWidget* HUD_Overlay;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets") //Enemy HealthBar -- da settare su BP
    TSubclassOf<class UUserWidget> WEnemyHealthBar;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
    UUserWidget* EnemyHealthBar;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets") //Pause menu -- da settare su BP
    TSubclassOf<class UUserWidget> WPauseMenu;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
    UUserWidget* PauseMenu;

   
    
    
    bool bEnemyHealthBarVisible;
    bool bPauseDisplayVisible;


    void DisplayEnemyHealthBar();
    void RemoveEnemyHealthBar();

    void TogglePauseMenu();

    //una parte in blueprint, una parte in c++
    //possono essere gestiti come eventi
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
    void DisplayPauseMenu();


    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
    void RemovePauseMenu();



    FVector EnemyLocation;
protected:

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

};
