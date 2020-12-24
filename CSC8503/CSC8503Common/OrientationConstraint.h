#pragma once
#pragma once
#include "Constraint.h"
#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8503 {
		class GameObject;

		using namespace Maths;

		class OrientationConstraint : public Constraint {
		public:
			OrientationConstraint(GameObject* a, GameObject* b, Vector3 axis) {
				objectA = a;
				objectB = b;
				this->axis = axis;
			}
			~OrientationConstraint() {};

			void UpdateConstraint(float dt) override;

		protected:
			GameObject* objectA;
			GameObject* objectB;

			Vector3 axis;
		};
	}
}