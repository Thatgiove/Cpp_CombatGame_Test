
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
    TSubclassOf<UUserWidget> WEnemyHealthBar;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
    UUserWidget* EnemyHealthBar;




    bool bEnemyHealthBarVisible;

    void DisplayEnemyHealthBar();
    void RemoveEnemyHealthBar();



    FVector EnemyLocation;
protected:

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

};
