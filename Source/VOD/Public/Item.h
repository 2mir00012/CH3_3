#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Item.generated.h"

// VOD 로그 카테고리 선언
DECLARE_LOG_CATEGORY_EXTERN(LogVOD, Warning, All);

UCLASS()
class VOD_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// 생성자
	AItem();

protected:
	// 루트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USceneComponent* SceneRoot;
	// 아이템 Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	UStaticMeshComponent* StaticMeshComp;
	// 아이템 Audio
	UAudioComponent* AudioComp;
	// 회전 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Properties")
	float RotationSpeed;
	// Component 초기화 후 호출
	virtual void PostInitializeComponents() override;
	// 게임 시작 시 호출
	virtual void BeginPlay() override;
	// 매 프레임 호출
	virtual void Tick(float DeltaTime) override;
	// 위치 초기화
	UFUNCTION(BlueprintCallable, Category = "Item|Actions")
	void ResetActorPosition();
	// 회전 속도 반환
	UFUNCTION(BlueprintPure, Category = "Item|Properties")
	float GetRotationSpeed() const;
	// Blueprint에서 구현할 획득 이벤트
	UFUNCTION(BlueprintImplementableEvent, Category = "Item|Events")
	void OnItemPickedUp();
	// Actor 제거 시 호출
	virtual void Destroyed() override;
	// Actor 종료 시 호출
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
