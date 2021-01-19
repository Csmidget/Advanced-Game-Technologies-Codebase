#include "RespawningObject.h"
#include <sstream>
#include <iomanip>
#include "../CSC8503Common/Debug.h"

using namespace NCL;
using namespace CSC8503;

RespawningObject::RespawningObject(Vector3 spawnPosition, string name, bool randomizeOrientation) : GameObject(name) {
	this->spawnPosition = spawnPosition;
	this->randomizeOrientation = randomizeOrientation;

	Respawn();
}

void RespawningObject::Respawn() {
	respawnTimer = 0.0f;

	transform.SetPosition(spawnPosition);

	if (physicsObject) {
		physicsObject->SetLinearVelocity(Vector3());
		physicsObject->SetAngularVelocity(Vector3());
	}

	if (!randomizeOrientation) {
		transform.SetOrientation({});
	}
	else {
		Quaternion newRot =
			Matrix3::Rotation(rand() % 360, Vector3(1, 0, 0)) *
			Matrix3::Rotation(rand() % 360, Vector3(0, 1, 0)) *
			Matrix3::Rotation(rand() % 360, Vector3(0, 0, 1));

		transform.SetOrientation(newRot);
	}

	OnRespawn();
}

void RespawningObject::Update(float dt) {

	float posOffset = (prevPos - transform.GetPosition()).Length();

	if (posOffset > 0.3f) {
		respawnTimer = 0.0f;
		prevPos = transform.GetPosition();
	}
	else {
		respawnTimer += dt;

		if (respawnTimer > 2.0f) {
			Respawn();
		}
	}
}

void RespawningObject::OnKill() {
	Respawn();
}

void RespawningObject::ObjectSpecificDebugInfo(int& currLine, float lineSpacing) {
	std::stringstream stream;

	stream << std::fixed << std::setprecision(2);

	stream << "Spawn point: " << spawnPosition;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");
}



