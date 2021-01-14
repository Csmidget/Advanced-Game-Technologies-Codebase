#include "AIObject.h"
#include "../../Common/Camera.h"
#include "../../Common/Window.h"
#include "RaceAIBehaviourTree.h";
#include "../../Common/Maths.h"
#include "../CSC8503Common/Debug.cpp"
#include "Game.h"

using namespace NCL;
using namespace CSC8503;

AIObject::AIObject(Game* game, Vector3 respawnPosition) : ActorObject(game, respawnPosition, "player") {
	behaviourTree = new RaceAIBehaviourTree(game, this);
	setGoalCooldown = 0.0f;
	currentGoal = respawnPosition;
	nextNode = respawnPosition;
}

AIObject::~AIObject() {

}

void AIObject::DisplayPath() {
	auto path = currentPath.GetWaypoints();
	Debug::DrawLine(transform.GetPosition(), nextNode + Vector3(0, 1, 0), Vector4(0, 1, 1, 1));

	if (path.empty())
		return;

	Debug::DrawLine(nextNode + Vector3(0,1,0), path.back() + Vector3(0,1,0), Vector4(0, 1, 0, 1));

	for (int i = 1; i < path.size(); ++i) {
		Vector3 a = path[i - 1] + Vector3(0, 1, 0);
		Vector3 b = path[i] + Vector3(0, 1, 0);

		Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
	}
}

bool AIObject::SetGoal(Vector3 newGoal, float maxCost) {

	if (newGoal == currentGoal) {
		return true;
	}

	//It is too soon to be changing goal again.
	if (setGoalCooldown > 0.0f) {
		return false;
	}

	NavigationPath newPath = game->GetPath(transform.GetPosition(), newGoal,maxCost);

	if (newPath.IsEmpty()) {
		return false;
	}

	setGoalCooldown = 1.0f;
	currentPath = newPath;
	currentPath.PopWaypoint(nextNode);
	currentGoal = newGoal;

	return true;
}

void AIObject::OnUpdate(float dt) {

	setGoalCooldown -= dt;
	setGoalCooldown = max(0.0f, setGoalCooldown);
	
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

	Vector3 xzPos = transform.GetPosition();
	xzPos.y = 0.0f;

	Vector3 direction = nextNode - xzPos;
	if (direction.Length() < 2.5f) {
		currentPath.PopWaypoint(nextNode);
	}

	direction.Normalise();
	physicsObject->AddForce(direction * 0.5f);

	DisplayPath();
}