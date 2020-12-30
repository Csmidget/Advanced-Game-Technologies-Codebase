#include "PlayerObject.h"
#include "../../Common/Camera.h"
#include "../../Common/Window.h"

using namespace NCL;
using namespace CSC8503;

PlayerObject::PlayerObject(Vector3 respawnPosition, Camera* playerCamera) : GameObject("player") {
	this->spawnPosition = respawnPosition;
	this->playerCam = playerCamera;
	this->controlEnabled = false;
	this->cameraPitch = 20.0f;
	this->cameraDistance = 10.0f;
}
PlayerObject::~PlayerObject() {

}

void PlayerObject::OnKill() {
	transform.SetPosition(spawnPosition);
	physicsObject->SetLinearVelocity(Vector3());
	physicsObject->SetAngularVelocity(Vector3());
}


void PlayerObject::Update(float dt) {

	lastCollisionTimer += dt;

	if (controlEnabled)  {
		
		cameraPitch += (Window::GetMouse()->GetRelativePosition().y);
		characterYaw -= Window::GetMouse()->GetRelativePosition().x;

		if (Window::GetMouse()->WheelMoved()) {
			cameraDistance -= Window::GetMouse()->GetWheelMovement();
		}

		if (lastCollisionTimer < 0.1f) {
			if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W)) {
				physicsObject->AddForce(transform.GetOrientation() * Vector3(0, 0, -3));
			}
			if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S)) {
				physicsObject->AddForce(transform.GetOrientation() * Vector3(0, 0, 3));
			}
			if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
				physicsObject->AddForce(transform.GetOrientation() * Vector3(-3, 0, 0));
			}
			if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
				physicsObject->AddForce(transform.GetOrientation() * Vector3(3, 0, 0));
			}
		}




		Quaternion orientation = Quaternion::EulerAnglesToQuaternion(0, characterYaw, 0);
		transform.SetOrientation(orientation);
		Vector3 angles = orientation.ToEuler();

		playerCam->SetPitch(-cameraPitch);
		playerCam->SetYaw(angles.y);

		Quaternion cameraAngle = Quaternion::EulerAnglesToQuaternion(-cameraPitch, angles.y, 0.0f);
		Vector3 cameraOffset = cameraAngle * (Vector3(0, 0, 1) * cameraDistance);
		Vector3 cameraFocusPoint = transform.GetPosition() + Vector3(0, 2, 0);
		playerCam->SetPosition(cameraFocusPoint + cameraOffset);

	//	if (Window::GetMouse()->)
	}
}

void PlayerObject::OnCollisionBegin(GameObject* otherObject) {
	lastCollisionTimer = 0.0f;
}
