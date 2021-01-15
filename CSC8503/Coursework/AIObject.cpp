#include "AIObject.h"
#include "../../Common/Camera.h"
#include "../../Common/Window.h"
#include "RaceAIBehaviourTree.h";
#include "../../Common/Maths.h"
#include "../CSC8503Common/Debug.cpp"
#include "Game.h"

#include <iomanip>
#include <sstream>

using namespace NCL;
using namespace CSC8503;

AIObject::AIObject(Game* game, Vector3 respawnPosition, float coinHuntRange, float angerThreshold, float strength) : ActorObject(game, respawnPosition, "ai") {
	behaviourTree = new RaceAIBehaviourTree(game, this);
	behaviourUpdateCooldown = 0.0f;
	currentGoal = respawnPosition;
	nextNode = respawnPosition;
	this->coinHuntRange = coinHuntRange;
	this->angerThreshold = angerThreshold;
	this->strength = strength;
	currentAnger = 0.0f;
}

AIObject::~AIObject() {

}

void AIObject::OnCollisionBegin(GameObject* otherObject) {

	if (otherObject->HasTag("actor")) {

		//If we're not angry yet, get angrier >:(
		if (currentAnger < angerThreshold) {
			++currentAnger;
		}
		//If we're angry, take it out on the other object.
		else {
			
			Vector3 direction = (otherObject->GetTransform().GetPosition() - transform.GetPosition() + Vector3(0, 1, 0)).Normalised();

			//FALCON PUNCH
			otherObject->GetPhysicsObject()->ApplyLinearImpulse(direction * strength);

			//That's better :)
			currentAnger = 0.0f;
		}

	}

	//When we collide with collectables we don't want to reset the collision timer...
	if (otherObject->GetPhysicsObject())
		lastCollisionTimer = 0.0f;

	ActorObject::OnCollisionBegin(otherObject);
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

	NavigationPath newPath = game->GetPath(transform.GetPosition(), newGoal,maxCost);

	if (newPath.IsEmpty()) {
		return false;
	}
	currentPath = newPath;
	currentPath.PopWaypoint(nextNode);
	currentGoal = newGoal;

	return true;
}

void AIObject::OnUpdate(float dt) {

	behaviourUpdateCooldown -= dt;
	behaviourUpdateCooldown = max(0.0f, behaviourUpdateCooldown);

	float angerColIntensity = currentAnger / angerThreshold;
	angerColIntensity = max(0.0f, angerColIntensity);
	renderObject->SetColour(Vector4(1, 1-angerColIntensity, 1-angerColIntensity, 1));
	
	if (behaviourUpdateCooldown < 0.0001f) {
		behaviourTree->Execute(dt);
		behaviourUpdateCooldown = 0.1f;
	}

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

	if (lastCollisionTimer < 0.1f) {
		Vector3 xzPos = transform.GetPosition();
		xzPos.y = 0.0f;

		Vector3 xzNextNode = nextNode;
		xzNextNode.y = 0.0f;

		Vector3 direction = xzNextNode - xzPos;
		if (direction.Length() < 2.5f) {
			currentPath.PopWaypoint(nextNode);
		}

		direction.Normalise();
		physicsObject->AddForce(direction * 4.0f);
	}

	physicsObject->SetAngularVelocity(Vector3(0, 0, 0));

#ifdef _DEBUG
	DisplayPath();
#endif
}

void AIObject::ObjectSpecificDebugInfo(int& currLine, float lineSpacing) {
	std::stringstream stream;

	DisplayPath();

	stream << std::fixed << std::setprecision(2);

	stream << "State: " << currentState;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

	stream << "Current Target: " << currentGoal;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

	stream << "Max Coin Distance: " << coinHuntRange;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");
}

void AIObject::OnRespawn() {
	if (!SetGoal(currentGoal)) {
		currentGoal = transform.GetPosition();
		nextNode = currentGoal;
		currentPath.Clear();
	}
}
