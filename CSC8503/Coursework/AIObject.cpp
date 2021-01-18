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

AIObject::AIObject(Game* game, Vector3 respawnPosition, std::string name, float coinHuntRange, float coinMaxDistance, float angerThreshold, float strength) : ActorObject(game, respawnPosition, name) {
	asleep = false;

	speed = 4.0f;
	currentAnger = 0.0f;
	behaviourUpdateCooldown = (float)(rand() % 1000) / 10000.0f;

	currentGoal = respawnPosition;
	nextNode = respawnPosition;

	this->coinHuntRange = coinHuntRange;
	this->coinMaxDistance = coinMaxDistance;
	this->angerThreshold = angerThreshold;
	this->strength = strength;

	behaviourTree = new RaceAIBehaviourTree(game, this);
}

AIObject::~AIObject() {

}

void AIObject::OnCollisionBegin(GameObject* otherObject) {

	if (!asleep && otherObject->HasTag("actor")) {

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
	
	//Position to next node
	Debug::DrawLine(transform.GetPosition(), nextNode + Vector3(0, 1, 0), Vector4(0, 1, 1, 1));

	if (path.empty())
		return;

	//Next node to first point in the path
	Debug::DrawLine(nextNode + Vector3(0,1,0), path.back() + Vector3(0,1,0), Vector4(0, 1, 0, 1));

	//Iterate through path
	for (int i = 1; i < path.size(); ++i) {
		Vector3 a = path[i - 1] + Vector3(0, 1, 0);
		Vector3 b = path[i] + Vector3(0, 1, 0);

		Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
	}
}

bool AIObject::SetGoal(Vector3 newGoal, float maxCost, bool force) {

	//If our goal is already set, we can ignore this request (Unless it is forced).
	if (newGoal == currentGoal && !force) {
		return true;
	}

	//Generate a path
	NavigationPath newPath = game->GetPath(transform.GetPosition(), newGoal,maxCost);

	//No path found
	if (newPath.IsEmpty()) {
		return false;
	}

	//Setup the new path
	currentPath = newPath;
	currentPath.PopWaypoint(nextNode);
	currentGoal = newGoal;

	return true;
}

void AIObject::OnUpdate(float dt) {

	//Don't update behaviour every frame.
	behaviourUpdateCooldown -= dt;
	behaviourUpdateCooldown = max(0.0f, behaviourUpdateCooldown);

	//Set the objects colour based on its current anger level.
	float angerColIntensity = currentAnger / angerThreshold;
	angerColIntensity = max(0.0f, angerColIntensity);
	renderObject->SetColour(Vector4(1, 1-angerColIntensity, 1-angerColIntensity, 1));
	

	if (!asleep && behaviourUpdateCooldown < 0.0001f) {
		behaviourTree->Execute(dt);
		behaviourUpdateCooldown = 0.1f;
	}

	//Get velocity on the x/z plane, this will inform the orientation (We don't want it to be looking up or down,
	//just straight forward.
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

		//We store an orientation value on the aiObject that won't be affected by physics.
		orientation = Matrix4::Rotation(angle, Vector3(0, 1, 0));
	}
	//Force the transform orientation to match the aiObject orientation
	transform.SetOrientation(orientation);
	//////////

	//Movement//
	if (!asleep && lastCollisionTimer < 0.1f) {
		Vector3 xzPos = transform.GetPosition();
		xzPos.y = 0.0f;

		Vector3 xzNextNode = nextNode;
		xzNextNode.y = 0.0f;

		Vector3 direction = xzNextNode - xzPos;
		if (direction.Length() < 2.5f) {
			currentPath.PopWaypoint(nextNode);
		}

		//We're a long distance from this node, we should regenerate the path
		//in case we've been knocked too far off course.
		if (direction.Length() > 7.5f) {
			SetGoal(currentGoal, INFINITY, true);
		}

		direction.Normalise();
		physicsObject->AddForce(direction * speed);
	}
	//////////////

	//Force angular velocity to 0. We don't want our character rolling around on the floor.
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

	stream << "Max Coin Distance: " << coinMaxDistance;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

	stream << "Anger Threshold " << angerThreshold;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

	stream << "Current Anger: " << currentAnger;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");
}

void AIObject::OnRespawn() {
	//On respawn we need to regenerate the ai's path.
	if (!SetGoal(currentGoal,INFINITY,true)) {
		currentGoal = transform.GetPosition();
		nextNode = currentGoal;
		currentPath.Clear();
	}
}
