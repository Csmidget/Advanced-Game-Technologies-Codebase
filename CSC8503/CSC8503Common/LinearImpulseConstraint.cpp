#include "LinearImpulseConstraint.h"
#include "GameObject.h"

using namespace NCL;
using namespace CSC8503;

void LinearImpulseConstraint::UpdateConstraint(float dt) {

	PhysicsObject* physA = object->GetPhysicsObject();

	physA->ApplyLinearImpulse(force / physA->GetInverseMass() * dt); //Will multiply by mass

}
