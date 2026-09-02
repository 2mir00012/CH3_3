#include "VODGameMode.h"
#include "VODCharacter.h"
#include "VODPlayerController.h"
#include "VODGameState.h"

AVODGameMode::AVODGameMode()
{
	// PlayerController 지정
	PlayerControllerClass = AVODPlayerController::StaticClass();
	// 기본 Player Character 지정
	DefaultPawnClass = AVODCharacter::StaticClass();
	// GameState 지정
	GameStateClass = AVODGameState::StaticClass();
}
