#include "ForceObject.h"
#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/LinearImpulseConstraint.h"

using namespace NCL;
using namespace CSC8503;

ForceObject::ForceObject(GameWorld* world, Vector3 position, Vector3 dimensions, Quaternion orientation, Vector3 direction, float strength, bool continuous) {
	this->name = "force object";
	this->world = world;
	this->direction = direction;
	this->strength = strength;
	this->continuous = continuous;
	OBBVolume* volume = new OBBVolume(dimensions);
	boundingVolume = (CollisionVolume*)volume;

	transform
		.SetPosition(position)
		.SetScale(dimensions * 2)
		.SetOrientation(orientation);

	physicsObject = new PhysicsObject(&transform, boundingVolume);
	physicsObject->SetInverseMass(0);
	physicsObject->InitCubeInertia();
	isStatic = true;
}

ForceObject::~ForceObject() {
	//Clear any constraints that still exist.
	for (auto pair : activeConstraints) {
		world->RemoveConstraint(pair.second, true);
	}
}

void ForceObject::OnCollisionBegin(GameObject* otherObject) {

	if (continuous) {
		Constraint* newConstraint = new LinearImpulseConstraint(otherObject, (transform.GetOrientation() * direction) * strength);
		world->AddConstraint(newConstraint);
		activeConstraints.emplace(otherObject, newConstraint);
	}
	else {
		PhysicsObject* otherPhys = otherObject->GetPhysicsObject();
		if (otherPhys) {
			otherPhys->ApplyLinearImpulse((transform.GetOrientation() * direction) * strength);
		}
	}

}

void ForceObject::OnCollisionEnd(GameObject* otherObject) {
	if (continuous) {
		world->RemoveConstraint(activeConstraints[otherObject], true);
		activeConstraints.erase(otherObject);
	}
}

