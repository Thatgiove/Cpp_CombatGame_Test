#include "Explosive.h"
#include "MainCharacter.h"

AExplosive::AExplosive()
{
	Damage = 15.f;
}


void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	if (OtherActor)
	{
		//cast playerPawn
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);

		MainCharacter->DecrementHealth(Damage);
	}
	//alla fine destroy actor 
	Destroy();

	//UE_LOG(LogTemp, Warning, TEXT("Explosive::OnOverlapEnd()"));

}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	UE_LOG(LogTemp, Warning, TEXT("Explosive::OnOverlapEnd()"));
}
