#include "KatamariState.h"
#include "PlayerObject.h"
#include "AIObject.h"
#include "DebugState.h"
#include "EndState.h"
#include "Game.h"
#include "../CSC8503Common/CollisionDetection.h"
#include "Checkpoint.h"

using namespace NCL::CSC8503;

KatamariState::KatamariState(Game* game) : GameState(game) {

	camera = game->GetWorld()->GetMainCamera();
	game->InitKatamariWorld();

	boidSwarm = new BoidSwarm(game);
	Vector3 start(-40.0, 2.0f, -40.0f);
	for (int x = 0; x < 16; x++) {
		for (int y = 0; y < 16; y++) {
			game->GetPrefabGenerator()->CreateBoid(game, &boidSwarm, start + Vector3(x*5,0,y*5));
		}
	}

	boidSwarm->Avoid(game->GetPlayerObject());

	for (auto opponent : game->GetOpponents()) {
		boidSwarm->Avoid((GameObject*)opponent);
	}
}

KatamariState::~KatamariState() {
	delete boidSwarm;
}

PushdownState::PushdownResult KatamariState::OnUpdate(float dt, PushdownState** newState) {

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

	Debug::Print("Catch balls and be the first to 50 points!", Vector2(15, 5), Vector4(1, 1, 0, 1), 1.5f);

	int playerScore = game->GetPlayerObject()->GetScore();

	if (playerScore >= 50) {
		*newState = new EndState(game, "You win!", "You were the first to reach 50 points!");
		return PushdownResult::Replace;
	}

	Debug::Print("Player: " + std::to_string(playerScore), Vector2(2, 20), Debug::YELLOW, 1.1f);
	auto opponents = game->GetOpponents();
	for (int i = 0; i < opponents.size(); ++i) {
		Debug::Print(opponents[i]->GetName() + ": " + std::to_string(opponents[i]->GetScore()), Vector2(2, 23 + i * 3),Debug::YELLOW,1.1f);

		if (opponents[i]->GetScore() >= 50) {
			*newState = new EndState(game, "You lose!", "One of your opponents caught 50 first!");
			return PushdownResult::Replace;
		}
	}



	//camera->UpdateCamera(dt);
	game->GetPlayerObject()->UpdateControls(camera);

	boidSwarm->Update();

	return PushdownResult::NoChange;
}

void KatamariState::OnAwake() {

}

void KatamariState::OnSleep() {

}