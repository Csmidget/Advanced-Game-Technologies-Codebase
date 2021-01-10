#include "PlayerObject.h"
#include "../../Common/Camera.h"
#include "../../Common/Window.h"

using namespace NCL;
using namespace CSC8503;

PlayerObject::PlayerObject(Game* game, Vector3 respawnPosition) : ActorObject(game, respawnPosition, "player") {
	lastCollisionTimer = 0.0f;
}

PlayerObject::~PlayerObject() {

}

void PlayerObject::OnUpdate(float dt) {
	lastCollisionTimer += dt;
}

void PlayerObject::UpdateControls() {
	Quaternion orientation = transform.GetOrientation();

	if (lastCollisionTimer < 0.1f) {
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W)) {
			physicsObject->AddForce(orientation * Vector3(0, 0, -3));
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S)) {
			physicsObject->AddForce(orientation * Vector3(0, 0, 3));
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
			physicsObject->AddForce(orientation * Vector3(-3, 0, 0));
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
			physicsObject->AddForce(orientation * Vector3(3, 0, 0));
		}
	}
}

void PlayerObject::OnCollisionBegin(GameObject* otherObject) {

	//When we collide with collectables we don't want to reset the collision timer...
	if (otherObject->GetPhysicsObject())
		lastCollisionTimer = 0.0f;
}
