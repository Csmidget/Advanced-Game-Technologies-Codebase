#include "PracticeState.h"
#include "PlayerObject.h"
#include "Game.h"

using namespace NCL::CSC8503;

PracticeState::PracticeState(Game* game) : GameState(game) {
	camera = game->GetWorld()->GetMainCamera();
	pitch = 20.0f;
	yaw = 0.0f;
	cameraDistance = 10.0f;
	game->ResetWorld();
}

PushdownState::PushdownResult PracticeState::OnUpdate(float dt, PushdownState** newState) {

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::BACK)) {
		return PushdownResult::Pop;
	}

	PlayerObject* player = game->GetPlayerObject();

	pitch += (Window::GetMouse()->GetRelativePosition().y);
	yaw -= Window::GetMouse()->GetRelativePosition().x;

	if (Window::GetMouse()->WheelMoved()) {
		cameraDistance -= Window::GetMouse()->GetWheelMovement();
	}

	//Update player object
	Quaternion orientation = Quaternion::EulerAnglesToQuaternion(0, yaw, 0);
	player->GetTransform().SetOrientation(orientation);
	player->UpdateControls();

	//Update camera
	Vector3 angles = orientation.ToEuler();

	camera->SetPitch(-pitch);
	camera->SetYaw(angles.y);

	Quaternion cameraAngle = Quaternion::EulerAnglesToQuaternion(-pitch, angles.y, 0.0f);
	Vector3 cameraOffset = cameraAngle * (Vector3(0, 0, 1) * cameraDistance);
	Vector3 cameraFocusPoint = player->GetTransform().GetPosition() + Vector3(0, 2, 0);
	camera->SetPosition(cameraFocusPoint + cameraOffset);

	return PushdownResult::NoChange;
}

void PracticeState::OnAwake() {

}

void PracticeState::OnSleep() {

}