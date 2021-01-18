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

	//We hit this after returning from the End Screen
	if (gameOver)
		return PushdownResult::Pop;

	//Resolve inputs
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
	auto opponents = game->GetOpponents();

	if (!game->IsPaused()) {
		scoreTracker += dt;

		if (scoreTracker > 1) {
			int scoreIncrement = std::floor(scoreTracker);

			if (!playerFinished) {
				player->AddScore(-scoreIncrement * 10);
			}

			for (auto opponent : opponents) {
				if (!opponent->IsAsleep()) {
					opponent->AddScore(-scoreIncrement * 10);
				}
			}
			scoreTracker -= scoreIncrement;
		}
	}

	std::sort(opponents.begin(), opponents.end(), [&](AIObject* a, AIObject* b){
		return a->GetScore() > b->GetScore();
	});

	//Display current scores
	Debug::Print("Player: " + std::to_string(player->GetScore()), Vector2(2, 5),playerFinished ? Debug::YELLOW : Debug::WHITE);

	for (int i = 0; i < opponents.size(); ++i) {
		Debug::Print(opponents[i]->GetName() + ": " + std::to_string(opponents[i]->GetScore()),Vector2(2,8 + i*3), opponents[i]->IsAsleep() ? Debug::YELLOW : Debug::WHITE);
	}

	bool opponentsFinished = true;
	for (auto opponent : opponents) {
		if (opponent->IsAsleep()) 
			continue;

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

	if (!playerFinished) {
		player->UpdateControls(camera);

		//Reached goal or ran out of score.
		if (game->GetGoal()->ReachedCheckpoint(player) || player->GetScore() <= 0) {
			playerFinished = true;
		}
	}
	else {
		camera->UpdateCamera(dt);
	}

	if (playerFinished && opponentsFinished) {
		gameOver = true;

		int playerPlacing = 0;
		for (auto opponent : opponents) {
			if (opponent->GetScore() > player->GetScore()) {
				++playerPlacing;
			}
			else {
				break;
			}
		}
		std::string winOrLose = (playerPlacing == 0 ? "You win!" : "You Lose!");

		const std::string placings[15]{"first","second","third","fourth","fifth","sixth","seventh","eigth","ninth","tenth","eleventh","twelth","thirteenth","fourteenth","fifteenth" };

		*newState = new EndState(game, winOrLose, "With a score of " + std::to_string(player->GetScore()) + " you came in " + placings[playerPlacing]);
		return PushdownResult::Push;

	}

	return PushdownResult::NoChange;
}

void RaceState::OnAwake() {

}

void RaceState::OnSleep() {

}