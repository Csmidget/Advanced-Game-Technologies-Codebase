#include "KatamariState.h"
#include "PlayerObject.h"
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
}

KatamariState::~KatamariState() {
	delete boidSwarm;
}

PushdownState::PushdownResult KatamariState::OnUpdate(float dt, PushdownState** newState) {

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::BACK)) {
		return PushdownResult::Pop;
	}

	Debug::Print("Catch balls and be the first to 30 points!", Vector2(15, 5), Vector4(1, 1, 0, 1), 1.5f);

	//camera->UpdateCamera(dt);
	game->GetPlayerObject()->UpdateControls(camera);

	boidSwarm->Update();

	return PushdownResult::NoChange;
}

void KatamariState::OnAwake() {

}

void KatamariState::OnSleep() {

}