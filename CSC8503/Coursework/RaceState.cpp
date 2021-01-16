#include "RaceState.h"
#include "AIObject.h"
#include "PlayerObject.h"
#include "DebugState.h"
#include "EndState.h"
#include "Game.h"
#include "../CSC8503Common/CollisionDetection.h"
#include "Checkpoint.h"
#include <algorithm>

using namespace NCL::CSC8503;

RaceState::RaceState(Game* game, int opponents) : GameState(game) {
	gameOver = false;
	camera = game->GetWorld()->GetMainCamera();
	game->InitRaceWorld(opponents);
	scoreTracker = 0.0f;
	game->GetPlayerObject()->AddScore(500.0f);

	for (auto opponent : game->GetOpponents()) {
		opponent->AddScore(500.0f);
	}
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

	auto opponents = game->GetOpponents();
	scoreTracker += dt;

	if (scoreTracker > 1) {
		int scoreIncrement = std::floor(scoreTracker);

		if (!goalReached) {
			player->AddScore(-scoreIncrement * 10);
		}

		for (auto opponent : opponents) {
			if (!opponent->IsAsleep()) {
				opponent->AddScore(-scoreIncrement * 10);
			}
		}
		scoreTracker -= scoreIncrement;
	}

	bool opponentsFinished = true;
	for (auto opponent : opponents) {

		if (opponent->IsAsleep())
			continue;
	
	}

	std::sort(opponents.begin(), opponents.end(), [&](AIObject* a, AIObject* b){
		return a->GetScore() > b->GetScore();
	});

	Debug::Print("Player: " + std::to_string(player->GetScore()), Vector2(5, 5),goalReached ? Debug::YELLOW : Debug::WHITE);

	for (int i = 0; i < opponents.size(); ++i) {
		Debug::Print(opponents[i]->GetName() + ": " + std::to_string(opponents[i]->GetScore()),Vector2(5,8 + i*3), opponents[i]->IsAsleep() ? Debug::YELLOW : Debug::WHITE);
	}

	for (auto opponent : opponents) {
		if (opponent->IsAsleep()) continue;

		if (opponent->GetScore() <= 0) {
			opponent->SetAsleep(true);
		}
		else if (game->GetGoal()->ReachedCheckpoint(opponent)) {
			opponent->SetAsleep(true);
		}
		else {
			opponentsFinished = false;
		}
	}

	if (!goalReached) {
		player->UpdateControls(camera);

		//Reached goal
		if (game->GetGoal()->ReachedCheckpoint(player)) {
			goalReached = true;
		}

		//Score hits 0
		if (player->GetScore() <= 0) {
			gameOver = true;
			*newState = new EndState(game, "You Lose!", "You ran out of score.");
			return PushdownResult::Push;
		}
	}
	else {
		camera->UpdateCamera(dt);
	}

	if (goalReached && opponentsFinished) {
		gameOver = true;
		*newState = new EndState(game, "You win!", "Well played.");
		return PushdownResult::Push;

	}

	return PushdownResult::NoChange;
}

void RaceState::OnAwake() {

}

void RaceState::OnSleep() {

}