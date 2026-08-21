#include "MyGameMode.h"
// 기본 Character로 사용할 MyCharacter
#include "MyCharacter.h"


AMyGameMode::AMyGameMode()
{
    // 게임 시작 시 사용할 기본 Character 클래스 지정
    DefaultPawnClass = AMyCharacter::StaticClass();
}
