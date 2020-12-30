#pragma once

#include "../CSC8503Common/GameObject.h"
#include "../../Common/Vector2.h"

namespace NCL {

	class Camera;

	namespace CSC8503 {

		class PlayerObject : public GameObject {

		public:
			PlayerObject(Vector3 respawnPosition, Camera* playerCamera);
			~PlayerObject();

			void OnCollisionBegin(GameObject* otherObject) override;
			void OnKill() override;
			void Update(float dt) override;

			bool GetControlEnabled() const {
				return controlEnabled;
			}
			void SetControlEnabled(bool val) {
				controlEnabled = val;
			}

		protected:

			Vector3 spawnPosition;
			float characterYaw;

			Camera* playerCam;
			//Pitch of the camera (It will inherit the players yaw, and will never roll)
			float cameraPitch;
			float cameraDistance;

			bool controlEnabled;

			float lastCollisionTimer;
		};
	}
}