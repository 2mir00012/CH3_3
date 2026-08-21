#include "KeyItem.h"

void AKeyItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 키 아이템 획득 메시지 출력
	UE_LOG(LogTemp, Warning, TEXT("Key collected!"));
}

void AKeyItem::ActivateItem(AActor* Activator)
{
	// 문 잠금 해제 메시지 출력
	UE_LOG(LogTemp, Warning, TEXT("Key used to unlock the door!"));
	// 사용된 키 아이템 제거
	DestroyItem();
}

