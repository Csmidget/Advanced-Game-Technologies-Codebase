#include "PauseState.h"
#include "Game.h"

using namespace NCL::CSC8503;

PauseState::PauseState(Game* game) : GameState(game) {
	world = game->GetWorld();
	camera = world->GetMainCamera();
	selectionMode = false;
	selectionObject = nullptr;
}

PauseState::~PauseState() {

	//clear colour change if an object is selected;
	if (selectionObject) {
		selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
	}

	Window::GetWindow()->ShowOSPointer(false);
	Window::GetWindow()->LockMouseToWindow(true);
}

PushdownState::PushdownResult PauseState::OnUpdate(float dt, PushdownState** newState) {

	Debug::Print("Press Q to change to camera mode!", Vector2(5, 85));

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::P)) {
		return PushdownResult::Pop;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) {

		if (selectionMode) {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
			selectionMode = false;
		}
		else {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
			selectionMode = true;
		}
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G)) {
		game->SetUseGravity(!game->GetUseGravity());//Toggle gravity!
	}

	if (game->GetUseGravity()) {
		Debug::Print("(G)ravity on", Vector2(5, 95));
	}
	else {
		Debug::Print("(G)ravity off", Vector2(5, 95));
	}

	if (!selectionMode) {
		camera->UpdateCamera(dt);
	}
	else {
		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
			if (selectionObject) {	//set colour to deselected;
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
				selectionObject = nullptr;
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(*camera);
			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true)) {
				Debug::DrawLine(ray.GetPosition(), closestCollision.collidedAt, Vector4(0, 1, 0, 1), 10.0f);
				selectionObject = (GameObject*)closestCollision.node;
				selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
			}
		}
	}


	return PushdownResult::NoChange;
}

void PauseState::OnAwake() {
	game->SetPause(true);
}

void PauseState::OnSleep() {
	game->SetPause(false);
}