#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

class AActor;

// 언리얼 엔진에서 인터페이스를 인식하기 위한 클래스
UINTERFACE(MinimalAPI)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

// 아이템 클래스가 구현할 함수 목록
class VOD_API IItemInterface
{
	GENERATED_BODY()

public:
	//Overlap 이벤트 바인딩을 위한 UFUNCTION
	UFUNCTION()
	// Begin Overlap 이벤트 형식에 맞게 매개변수 변경
	virtual void OnItemOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) = 0;
	// End Overlap 이벤트 바인딩을 위한 UFUNCTION
	UFUNCTION()
	//  End Overlap 이벤트 형식에 맞게 매개변수 변경
	virtual void OnItemEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) = 0;
	// 아이템이 사용되었을 때 호출
	virtual void ActivateItem(AActor* Activator) = 0;
	// 아이템 종류 반환
	virtual FName GetItemType() const = 0;
};
