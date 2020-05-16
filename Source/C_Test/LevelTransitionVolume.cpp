#include "LevelTransitionVolume.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "MainCharacter.h"

ALevelTransitionVolume::ALevelTransitionVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Volume"));
	//BoxComponent->SetupAttachment(GetRootComponent());
	RootComponent = BoxComponent;

	BillBordComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillBoard"));
	BillBordComponent->SetupAttachment(GetRootComponent());


	TransitionLevelName = "";

}

void ALevelTransitionVolume::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionVolume::OnOverlapBegin);
}
//void ALevelTransitionVolume::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}


void ALevelTransitionVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main)
		{
			Main->ChangeLevel(TransitionLevelName);
		}
	}

}


