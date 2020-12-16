#include "Treadmill.h"
#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/ForceConstraint.h"

using namespace NCL;
using namespace CSC8503;

Treadmill::Treadmill(GameWorld* world, MeshGeometry* mesh, TextureBase* tex, ShaderBase* shader, Vector3 position, Vector3 dimensions, Vector3 direction) {
	this->world = world;
	this->direction = direction;

	AABBVolume* volume = new AABBVolume(dimensions);
	boundingVolume = (CollisionVolume*)volume;

	transform.SetPosition(position).SetScale(dimensions * 2);

	renderObject = new RenderObject(&transform, mesh, tex, shader);
	renderObject->SetColour(Vector4(0.2f, 0.2f, 0.2f, 1.0f));

	physicsObject = new PhysicsObject(&transform, boundingVolume);
	physicsObject->SetInverseMass(0);
	physicsObject->InitCubeInertia();

	isStatic = true;
}

void Treadmill::OnCollisionBegin(GameObject* otherObject) {

	Constraint* newConstraint = new ForceConstraint(otherObject, transform.GetOrientation() * direction);
	world->AddConstraint(newConstraint);
	activeConstraints.emplace(otherObject, newConstraint);

}
void Treadmill::OnCollisionEnd(GameObject* otherObject) {
	world->RemoveConstraint(activeConstraints[otherObject], true);
	activeConstraints.erase(otherObject);
}

Treadmill::~Treadmill() {
	//Clear any constraints that still exist.
	for (auto pair : activeConstraints) {
		world->RemoveConstraint(pair.second, true);
	}
}