#include "PauseState.h"
#include "Game.h"

using namespace NCL::CSC8503;

PauseState::PauseState(Game* game) : GameState(game) {
	camera = game->GetWorld()->GetMainCamera();
}

PushdownState::PushdownResult PauseState::OnUpdate(float dt, PushdownState** newState) {


	return PushdownResult::NoChange;
}

void PauseState::OnAwake() {
	game->SetPause(true);
}

void PauseState::OnSleep() {
	game->SetPause(false);
}