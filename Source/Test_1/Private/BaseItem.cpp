#include "BaseItem.h"
// SphereComponent 사용을 위한 헤더
#include "Components/SphereComponent.h"

ABaseItem::ABaseItem()
{
	// 매 프레임 실행할 기능이 없으므로 Tick 비활성화
	PrimaryActorTick.bCanEverTick = false;
	// 루트 컴포넌트 생성 및 설정
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	// 충돌 컴포넌트 생성
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	// [추가] 겹침만 감지하는 프로파일 설정
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	// Collision을 Scene에 연결
	Collision->SetupAttachment(Scene);
	// 스태틱 메시 컴포넌트 생성 및 연결
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);
	// Begin Overlap 이벤트 연결
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	//] End Overlap 이벤트 연결
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);
}
void ABaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//  OtherActor가 플레이어인지 확인
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		//  Overlap 디버그 메시지
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!!")));
		//  아이템 사용 로직 호출
		ActivateItem(OtherActor);
	}
}
void ABaseItem::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
void ABaseItem::ActivateItem(AActor* Activator)
{
	//  Overlap 디버그 메시지
	GEngine->AddOnScreenDebugMessage(-1,	2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!")));
}
FName ABaseItem::GetItemType() const
{
	// 현재 아이템의 종류 반환
	return ItemType;
}
void ABaseItem::DestroyItem()
{
	// 아이템 액터 제거
	Destroy();
}
