#include "RaceState.h"
#include "PlayerObject.h"
#include "PauseState.h"
#include "EndState.h"
#include "Game.h"
#include "../CSC8503Common/CollisionDetection.h"

#include <algorithm>

using namespace NCL::CSC8503;

RaceState::RaceState(Game* game) : GameState(game) {
	camera = game->GetWorld()->GetMainCamera();
	pitch = 20.0f;
	yaw = 0.0f;
	cameraDistance = 10.0f;
	game->InitRaceWorld(1);
	scoreTracker = 0.0f;
}

PushdownState::PushdownResult RaceState::OnUpdate(float dt, PushdownState** newState) {

	if (gameOver)
		return PushdownResult::Pop;

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::BACK)) {
		return PushdownResult::Pop;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::P)) {
		*newState = new PauseState(game);
		return PushdownResult::Push;
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

	pitch += (Window::GetMouse()->GetRelativePosition().y);
	pitch = max(-15.0f, min(90.0f, pitch));
	yaw -= Window::GetMouse()->GetRelativePosition().x;

	if (Window::GetMouse()->WheelMoved()) {
		cameraDistance -= Window::GetMouse()->GetWheelMovement();
		cameraDistance = max(5.0f, cameraDistance);
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