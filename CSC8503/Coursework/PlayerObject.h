#pragma once
#include "ActorObject.h"
#include "../../Common/Vector2.h"

namespace NCL {

	namespace CSC8503 {

		class PlayerObject : public ActorObject {

		public:
			PlayerObject(Game* game, Vector3 respawnPosition);
			~PlayerObject();

			void OnUpdate(float dt) override;
			void UpdateControls();

		protected:
			float lastCollisionTimer;
		};
	}
}