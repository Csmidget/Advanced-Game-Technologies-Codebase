#include "PendulumObject.h"
#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/LinearImpulseConstraint.h"

using namespace NCL::CSC8503;


PendulumObject::PendulumObject(GameWorld* world, GameObject* anchor, float distance, Vector3 force) {
	this->world = world;
	this->distance = distance;
	this->anchor = anchor;
	this->force = force;
	invert = false;
	aboveAnchor = false;
	constraint = new LinearImpulseConstraint(this, force);
}

void PendulumObject::Update(float dt) {

//	float offset

//	float distFromPlane = tangentPlane.DistanceFromPlane(transform.GetPosition());

	//if (distFromPlane > distFromPlane / 2) {
	//	world->RemoveConstraint(constraint, true);
	//	constraint = new LinearImpulseConstraint(this, force);
	//	world->AddConstraint(constraint);
	//}
	//else if (-distFromPlane > distFromPlane / 2) {
	//	world->RemoveConstraint(constraint, true);
	//	constraint = new LinearImpulseConstraint(this, -force);
	//	world->AddConstraint(constraint);
	//}
}