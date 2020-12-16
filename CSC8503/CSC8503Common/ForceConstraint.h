#pragma once
#include "Constraint.h"
#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8503 {
		class GameObject;
		
		using namespace Maths;

		class ForceConstraint : public Constraint {
		public:
			ForceConstraint(GameObject* obj, Vector3 force) {
				object = obj;
				this->force = force;
			}
			~ForceConstraint() {};

			void UpdateConstraint(float dt) override;

		protected:
			GameObject* object;
			Vector3 force;

		};
	}
}