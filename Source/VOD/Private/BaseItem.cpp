#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"

ABaseItem::ABaseItem()
{
	// Tick 비활성화
	PrimaryActorTick.bCanEverTick = false;
	// 루트 컴포넌트 생성
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	// 충돌 컴포넌트 생성
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Scene);
	// 아이템 Mesh 생성
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);
	// Overlap 이벤트 연결
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);
	// 획득 효과 초기화
	PickupParticle = nullptr;
	PickupSound = nullptr;
}
void ABaseItem::OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Player와 충돌했는지 확인
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		// Overlap 확인 메시지
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!!")));
		// 아이템 효과 실행
		ActivateItem(OtherActor);
	}
}
void ABaseItem::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
void ABaseItem::ActivateItem(AActor* Activator)
{
	// 생성된 파티클 저장
	UParticleSystemComponent* Particle = nullptr;
	// 획득 파티클 생성
	if (PickupParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickupParticle, GetActorLocation(), GetActorRotation(), true);
	}
	// 획득 사운드 재생
	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
	}
	// 파티클이 생성되었다면 일정 시간 후 제거
	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;
		TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;
		GetWorld()->GetTimerManager().SetTimer(DestroyParticleTimerHandle, [WeakParticle]()
			{
				if (WeakParticle.IsValid())
				{
					WeakParticle->DestroyComponent();
				}
			},
			2.0f, false
		);
	}
	// 아이템 작동 확인 메시지
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!")));
}
FName ABaseItem::GetItemType() const
{
	// 아이템 종류 반환
	return ItemType;
}
void ABaseItem::DestroyItem()
{
	// 아이템 제거
	Destroy();
}
