#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "VODGameState.h"

AMineItem::AMineItem()
{
	// 폭발 설정
	ExplosionDelay = 0.1f;
	ExplosionRadius = 300.0f;
	ExplosionDamage = 50.0f;
	// 아이템 종류
	ItemType = "Mine";
	// 폭발 상태 초기화
	bHasExploded = false;
	// 폭발 효과 초기화
	ExplosionParticle = nullptr;
	ExplosionSound = nullptr;
	// 폭발 범위 생성
	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	// 폭발 범위 설정
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	// Overlap 충돌 설정
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	// 루트에 연결
	ExplosionCollision->SetupAttachment(Scene);
}
void AMineItem::ActivateItem(AActor* Activator)
{
	// 이미 작동한 지뢰는 무시
	if (bHasExploded)
	{
		return;
	}
	// 공통 획득 효과 실행
	Super::ActivateItem(Activator);
	// 일정 시간 후 폭발
	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &AMineItem::Explode, ExplosionDelay);
	// 중복 작동 방지
	bHasExploded = true;
}
void AMineItem::Explode()
{
	// 생성된 폭발 파티클 저장
	UParticleSystemComponent* Particle = nullptr;
	// 폭발 파티클 생성
	if (ExplosionParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation(), GetActorRotation(), false);
	}
	// 폭발 사운드 재생
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}
	// 폭발 범위 안 Actor 가져오기
	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);
	// 범위 안의 Player에게 데미지 적용
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag(TEXT("Player")))
		{
			UGameplayStatics::ApplyDamage(Actor, ExplosionDamage, nullptr, this, UDamageType::StaticClass());
		}
	}
	// 지뢰 폭발을 GameState에 전달
	AVODGameState* VODGameState = GetWorld()
		? GetWorld()->GetGameState<AVODGameState>()
		: nullptr;
	if (VODGameState)
	{
		VODGameState->OnMineExploded();
	}
	// 지뢰 제거
	DestroyItem();
	// 폭발 파티클 제거
	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;
		TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;
		// 1초 후 파티클 제거
		GetWorld()->GetTimerManager().SetTimer(DestroyParticleTimerHandle, [WeakParticle]()
			{
				if (WeakParticle.IsValid())
				{
					WeakParticle->DestroyComponent();
				}
			},
			1.0f, false
		);
	}
}