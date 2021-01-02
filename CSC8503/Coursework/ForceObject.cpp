#include "ForceObject.h"
#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/LinearImpulseConstraint.h"

using namespace NCL;
using namespace CSC8503;

ForceObject::ForceObject(GameWorld* world, MeshGeometry* mesh, TextureBase* tex, ShaderBase* shader, Vector3 position, Vector3 dimensions, Quaternion orientation, Vector3 direction, float strength) {
	this->name = "force object";
	this->world = world;
	this->direction = direction;
	this->strength = strength;
	OBBVolume* volume = new OBBVolume(dimensions);
	boundingVolume = (CollisionVolume*)volume;

	transform
		.SetPosition(position)
		.SetScale(dimensions * 2)
		.SetOrientation(orientation);

	renderObject = new RenderObject(&transform, mesh, tex, shader);
	renderObject->SetColour(Vector4(0.2f, 0.2f, 0.2f, 1.0f));

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

	Constraint* newConstraint = new LinearImpulseConstraint(otherObject, transform.GetOrientation() * direction * strength);
	world->AddConstraint(newConstraint);
	activeConstraints.emplace(otherObject, newConstraint);

}
void ForceObject::OnCollisionEnd(GameObject* otherObject) {
	world->RemoveConstraint(activeConstraints[otherObject], true);
	activeConstraints.erase(otherObject);
}

