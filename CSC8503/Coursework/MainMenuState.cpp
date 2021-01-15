#include "MainMenuState.h"
#include "PracticeState.h"
#include "RaceState.h"
#include "Game.h"
#include "../CSC8503Common/GameWorld.h"

using namespace NCL::CSC8503;

MainMenuState::MainMenuState(Game* game) : GameState(game) {
	selection = 0;
	world = game->GetWorld();
};

PushdownState::PushdownResult MainMenuState::OnUpdate(float dt, PushdownState** newState) {
	Camera* cam = world->GetMainCamera();
	Debug::Print("Main Menu", Vector2(5, 20), Debug::YELLOW,1.5f);
	Debug::Print("Practice", Vector2(5, 30), selection == 0 ? Debug::GREEN : Debug::WHITE,1.5f);
	Debug::Print("Versus", Vector2(5, 40), selection == 1 ? Debug::GREEN : Debug::WHITE,1.5f);
	Debug::Print("Quit", Vector2(5, 50), selection == 2 ? Debug::GREEN : Debug::WHITE,1.5f);

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::UP)) {
		--selection;
		if (selection < 0)
			selection = 2;
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::DOWN)) {
		selection = (selection + 1) % 3;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::RETURN)) {
		switch (selection) {
		case 0:
			*newState = new PracticeState(game);
			return PushdownResult::Push;
		case 1:
			*newState = new RaceState(game);
			return PushdownResult::Push;

		case 2:
			return PushdownResult::Pop;
		}
	}
	cameraYaw += dt * 2;

	cam->SetPosition( Quaternion(Matrix4::Rotation(cameraYaw,Vector3(0,1,0))) * Vector3(0, 0,250) + Vector3(0,150,0));
	cam->SetPitch(-30);
	cam->SetYaw(cameraYaw);

	return PushdownResult::NoChange;
}

void MainMenuState::OnAwake() {
	cameraYaw = 0;
	game->InitPracticeWorld();
}