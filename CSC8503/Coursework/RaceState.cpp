#include "RaceState.h"
#include "PlayerObject.h"
#include "DebugState.h"
#include "EndState.h"
#include "Game.h"
#include "../CSC8503Common/CollisionDetection.h"

#include <algorithm>

using namespace NCL::CSC8503;

RaceState::RaceState(Game* game, int opponents) : GameState(game) {
	gameOver = false;
	camera = game->GetWorld()->GetMainCamera();
	game->InitRaceWorld(opponents);
	scoreTracker = 0.0f;
	game->GetPlayerObject()->AddScore(100000.0f);

}

PushdownState::PushdownResult RaceState::OnUpdate(float dt, PushdownState** newState) {

	if (gameOver)
		return PushdownResult::Pop;

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::BACK)) {
		return PushdownResult::Pop;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::TAB)) {
		*newState = new DebugState(game);
		return PushdownResult::Push;
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::P)) {
		game->SetPause(!game->IsPaused());
	}


	PlayerObject* player = game->GetPlayerObject();

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM1)) {
		player->GetTransform().SetPosition(Vector3(-100, 10, 100));
	}
	else if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM2)) {
		player->GetTransform().SetPosition(Vector3(-100, 10, -100));
	}
	else if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM3)) {
		player->GetTransform().SetPosition(Vector3(100, 110.0f, -100));
	}

	scoreTracker += dt;

	if (scoreTracker > 1) {
		int scoreIncrement = std::floor(scoreTracker);
		player->AddScore(-scoreIncrement * 10);
		scoreTracker -= scoreIncrement;
	}

	Debug::Print("Score: " + std::to_string(player->GetScore()), Vector2(5, 5));

	player->UpdateControls(camera);

	Vector3 playerAABB;

	//Reached goal
	if (player->GetBroadphaseAABB(playerAABB) && CollisionDetection::AABBTest(player->GetTransform().GetPosition(), game->GetGoal()->GetPosition(), playerAABB, game->GetGoal()->GetHalfDims())) {
		gameOver = true;
		*newState = new EndState(game, "You Win!", "Score: " + std::to_string(player->GetScore()));
		return PushdownResult::Push;
	}

	//Score hits 0
	if (player->GetScore() <= 0) {
		gameOver = true;
		*newState = new EndState(game, "You Lose!", "You ran out of score.");
		return PushdownResult::Push;
	}

	return PushdownResult::NoChange;
}

void RaceState::OnAwake() {

}

void RaceState::OnSleep() {

}