#include "VODGameMode.h"
// 기본 Character로 사용할 MyCharacter
#include "VODCharacter.h"


AVODGameMode::AVODGameMode()
{
    // 게임 시작 시 사용할 기본 Character 클래스 지정
    DefaultPawnClass = AVODCharacter::StaticClass();
}
