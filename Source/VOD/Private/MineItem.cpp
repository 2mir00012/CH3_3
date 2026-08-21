#include "MineItem.h"
// ExplosionCollision 사용을 위한 헤더
#include "Components/SphereComponent.h"

AMineItem::AMineItem()
{
	// 지뢰의 기본 폭발 값 설정
	ExplosionDelay = 5.0f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 30.0f;
	ItemType = "Mine";
	// 폭발 범위용 SphereComponent 생성
	ExplosionCollision = 
CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	// 폭발 범위 설정
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	// 폭발 콜리전 프로파일 설정
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	// Scene에 폭발 콜리전 연결
	ExplosionCollision->SetupAttachment(Scene);
}

void AMineItem::ActivateItem(AActor* Activator)
{
	// 즉시 제거하지 않고 5초 후 Explode 호출
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AMineItem::Explode, ExplosionDelay);
}
// 폭발 처리 함수
void AMineItem::Explode()
{
	// 폭발 범위와 겹친 액터 저장
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);
	// 폭발 범위 안의 액터 확인
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			// 지뢰 데미지 디버그 메시지
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Player damaged %d by MineItem"), ExplosionDamage));
		}
	}
	// 사용된 지뢰 제거
	DestroyItem();
}

