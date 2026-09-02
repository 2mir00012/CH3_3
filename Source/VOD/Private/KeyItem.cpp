#include "KeyItem.h"

void AKeyItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 키 획득 로그
	UE_LOG(LogTemp, Warning, TEXT("Key collected!"));
}
void AKeyItem::ActivateItem(AActor* Activator)
{
	// 키 사용 로그
	UE_LOG(LogTemp, Warning, TEXT("Key used to unlock the door!"));
	// 키 제거
	DestroyItem();
}

