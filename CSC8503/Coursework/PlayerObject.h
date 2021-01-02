#pragma once

#include "RespawningObject.h"
#include "../../Common/Vector2.h"

namespace NCL {

	class Camera;

	namespace CSC8503 {

		class PlayerObject : public RespawningObject {

		public:
			PlayerObject(Vector3 respawnPosition);
			~PlayerObject();

			void Update(float dt) override;
			void UpdateControls();
			void OnCollisionBegin(GameObject* otherObject) override;

		protected:
			float lastCollisionTimer;
		};
	}
}