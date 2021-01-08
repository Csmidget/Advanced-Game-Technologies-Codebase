#include "EndState.h"
#include "Game.h"
#include "../CSC8503Common/Debug.h"

using namespace NCL::CSC8503;

PushdownState::PushdownResult EndState::OnUpdate(float dt, PushdownState** newState) {

	Debug::Print(mainText, Vector2(15, 30));
	Debug::Print(subText, Vector2(15, 40));

	Debug::Print("Press space to return to the main menu.", Vector2(15, 60));

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
		return PushdownResult::Pop;
	}

	return PushdownResult::NoChange;
}


void EndState::OnAwake() {
	game->SetPause(true);
}

void EndState::OnSleep() {
	game->SetPause(false);
}