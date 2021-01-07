#pragma once

#include "ActorObject.h"
#include "../../Common/Vector2.h"

namespace NCL {

	class Camera;

	namespace CSC8503 {

		class PlayerObject : public ActorObject {

		public:
			PlayerObject(GameWorld* world, Vector3 respawnPosition);
			~PlayerObject();

			void OnUpdate(float dt) override;
			void UpdateControls();
			void OnCollisionBegin(GameObject* otherObject) override;

		protected:
			float lastCollisionTimer;
		};
	}
}