
#include "AMyTriggerVolume.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

AAMyTriggerVolume::AAMyTriggerVolume()
{
	PrimaryActorTick.bCanEverTick = true;


	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;  //il  TriggerVolume è il root component

	//parametru per la collisione
	//solo overlap events, non ha a che fare con la fisica
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic); //tipo di obj  
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //alla collisione risponde igorando gli oggetti  di default
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);//alla collisione col pawn  risponde con evento di overlap

	TriggerBox->SetBoxExtent(FVector(65.f, 65.f, 32.f)); //creiamo il box


	FloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSwitch"));
	FloorSwitch->SetupAttachment(GetRootComponent());


	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));;
	Door->SetupAttachment(GetRootComponent());




}

void AAMyTriggerVolume::BeginPlay()
{
	Super::BeginPlay();

	//posizione iniziale della static mesh
	InitialDoorLocation = Door->GetComponentLocation();
	InitialSwitchLocation = FloorSwitch->GetComponentLocation();



	//prendiamo la struct
	 //per essere bindata al delegato la funzione deve avere gli stessi parametri 
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AAMyTriggerVolume::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AAMyTriggerVolume::OnOverlapEnd);


	SwitchTime = 2.f;
	bCharacterOnSwitch = true;
}

void AAMyTriggerVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//informazioni che vengono passate quando si verifica una collisione


void AAMyTriggerVolume::OnOverlapBegin
(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	UE_LOG(LogTemp, Warning, TEXT("On Overlap Begin"));
	//funzione definita in Blueprint
	if (!bCharacterOnSwitch) bCharacterOnSwitch = true;

		RaiseDoor();
		LowerFloorSwitch();

	
}


void AAMyTriggerVolume::OnOverlapEnd
(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	
	if (bCharacterOnSwitch) bCharacterOnSwitch = false;
	UE_LOG(LogTemp, Warning, TEXT("On Overlap End"));
	//per settare il timer
	/*Prende in input il FTimerHandle,
	la classe a cui è assegnato e il puntatore alla funzione da eseguire e il tempo in secondi*/
	GetWorldTimerManager().SetTimer(SwitchTimerHandle, this, &AAMyTriggerVolume::CloseDoor, SwitchTime);




}

void AAMyTriggerVolume::UpdateDoorLocation(float ZValue)
{
	/*Ad ogni tick aggiunge a Z lo ZValue */

	FVector NewLocation = InitialDoorLocation;
	NewLocation.Z += ZValue;

	Door->SetWorldLocation(NewLocation);
}

void AAMyTriggerVolume::CloseDoor()
{
	//non ha senso alzare la piattaforma se il personaggio è sopra di essa
	if (!bCharacterOnSwitch)
	{
		LowerDoor();
		RaiseFloorSwitch();
	}

}
