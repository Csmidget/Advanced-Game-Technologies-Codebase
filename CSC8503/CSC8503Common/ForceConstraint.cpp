#include "ForceConstraint.h"
#include "GameObject.h"

using namespace NCL;
using namespace CSC8503;

void ForceConstraint::UpdateConstraint(float dt) {

	PhysicsObject* physA = object->GetPhysicsObject();

	physA->ApplyLinearImpulse(force * dt); //Will multiply by mass

}
