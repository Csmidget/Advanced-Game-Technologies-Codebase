#include "AIObject.h"
#include "../../Common/Camera.h"
#include "../../Common/Window.h"
#include "RaceAIBehaviourTree.h";
#include "../../Common/Maths.h"

using namespace NCL;
using namespace CSC8503;

AIObject::AIObject(Game* game, Vector3 respawnPosition) : ActorObject(game, respawnPosition, "player") {
	lastCollisionTimer = 0.0f;
	behaviourTree = new RaceAIBehaviourTree(game, this);
}

AIObject::~AIObject() {

}

void AIObject::OnUpdate(float dt) {
	lastCollisionTimer += dt;
	
	behaviourTree->Execute(dt);

	Vector3 vel = physicsObject->GetLinearVelocity();
	vel.y = 0.0f;
	float epsilon = 0.1f;
	
	//If the character is moving in a direction, turn them to face the direction of travel.
	if (vel.Length() > epsilon) {
		vel.Normalise();
		Vector3 forward = Vector3(0, 0, -1);

		//dot prod gives us cos(angle between vectors), acos gives us the angle in radians.
		float angle = acos(Vector3::Dot(forward, vel));

		//Might be able to avoid this step but the Matrix4::Rotation() function takes angle in degrees.
		angle = Maths::RadiansToDegrees(angle);

		//Flip the direction of the angle if the cross prod is facing away from the up vector
		if (Vector3::Dot(Vector3(0, 1, 0), Vector3::Cross(forward, vel)) < 0)
			angle = -angle;

		orientation = Matrix4::Rotation(angle, Vector3(0, 1, 0));
	}

	transform.SetOrientation(orientation);
}

void AIObject::OnCollisionBegin(GameObject* otherObject) {

	//When we collide with collectables we don't want to reset the collision timer...
	if (otherObject->GetPhysicsObject())
		lastCollisionTimer = 0.0f;
}
