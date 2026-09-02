#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

class AActor;

// Unreal에서 사용할 Interface 클래스
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

// 아이템 공통 기능 정의
class VOD_API IItemInterface
{
	GENERATED_BODY()

public:
	// Overlap 시작
	UFUNCTION()
	virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) = 0;
	// Overlap 종료
	UFUNCTION()
	virtual void OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) = 0;
	// 아이템 효과 실행
	virtual void ActivateItem(AActor* Activator) = 0;
	// 아이템 종류 반환
	virtual FName GetItemType() const = 0;
};
