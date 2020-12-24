#include "OrientationConstraint.h"
#include "GameObject.h"
#include "../CSC8503Common/Debug.h"

using namespace NCL;
using namespace CSC8503;

void OrientationConstraint::UpdateConstraint(float dt) {
	Vector3 relativePosAxis = (objectA->GetTransform().GetPosition() -
		objectB->GetTransform().GetPosition()).Normalised();

	Vector3 bAxis = (objectB->GetTransform().GetOrientation() * axis).Normalised();

	Vector3 rotationAxis = Vector3::Cross(relativePosAxis, bAxis);


	if (rotationAxis.Length() > 0.0f) {

		PhysicsObject* physA = objectA->GetPhysicsObject();
		PhysicsObject* physB = objectB->GetPhysicsObject();

		Vector3 relativeVelocity = physA->GetAngularVelocity() -
			physB->GetAngularVelocity(); 

		float constraintMass = physA->GetInverseMass() +
			physB->GetInverseMass();

		if (constraintMass > 0.0f) {
			//how much of their relative force is affecting the constraint
			float velocityDot = Vector3::Dot(relativeVelocity, rotationAxis);

			float biasFactor = 0.1f;
			float bias = -(biasFactor / dt) * rotationAxis.Length();
			float lambda = -(velocityDot + bias) / constraintMass;

			Vector3 aImpulse = rotationAxis * lambda;
			Vector3 bImpulse = -rotationAxis * lambda;

			physA->ApplyAngularImpulse(aImpulse); //Will multiply by mass
			physB->ApplyAngularImpulse(bImpulse); //Will multiply by mass
		}

		Debug::DrawLine(objectB->GetTransform().GetPosition(), objectB->GetTransform().GetPosition() + bAxis * 10);
	}
}
