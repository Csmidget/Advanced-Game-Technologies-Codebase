#pragma once

#include "RespawningObject.h"
#include "../../Common/Vector2.h"
#include "../../Common/Plane.h"


namespace NCL {
	namespace CSC8503 {

		class GameWorld;
		class LinearImpulseConstraint;

		class PendulumObject : public GameObject {

		public:
			PendulumObject(GameWorld* world, GameObject* anchor, float distance, Vector3 force);
			~PendulumObject() {};

			void Update(float dt) override;

		protected:

			LinearImpulseConstraint* constraint;
			bool invert;
			bool aboveAnchor;
			float distance;
			Vector3 force;
			GameWorld* world;
			GameObject* anchor;
		};
	}
}