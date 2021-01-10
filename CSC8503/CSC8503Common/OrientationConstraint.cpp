#include "OrientationConstraint.h"
#include "GameObject.h"
#include "../CSC8503Common/Debug.h"

using namespace NCL;
using namespace CSC8503;

void OrientationConstraint::UpdateConstraint(float dt) {

	Vector3 targetOrientation = (targetObject->GetTransform().GetPosition() -
		object->GetTransform().GetPosition()).Normalised();

	Vector3 currentOrientation = (object->GetTransform().GetOrientation() * axis).Normalised();

	Vector3 rotationAxis = Vector3::Cross(targetOrientation, currentOrientation);

	//Length of axis indicates how far out of alignment the target orientation and current orientation are
	if (rotationAxis.Length() > 0.0f) {

		PhysicsObject* physObject = object->GetPhysicsObject();

		float constraintMass = physObject->GetInverseMass();

		if (constraintMass > 0.0f) {
			//how much of the current angular velocity is affecting the constraint axis
			float velocityDot = Vector3::Dot(physObject->GetAngularVelocity(), rotationAxis.Normalised());

			float biasFactor = 0.1f;

			float bias = (biasFactor / dt) * rotationAxis.Length();
			float lambda = -(velocityDot + bias) / constraintMass;
			Vector3 impulse = rotationAxis * lambda;

			physObject->ApplyAngularImpulse(impulse); //Will multiply by intertia tensor
		}
	}
}
