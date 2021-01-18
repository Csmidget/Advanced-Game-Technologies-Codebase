#pragma once
#include "ActorObject.h"
#include "../../Common/Vector2.h"

namespace NCL {

	class Camera; 

	namespace CSC8503 {


		class PlayerObject : public ActorObject {

		public:
			PlayerObject(Game* game, Vector3 respawnPosition);
			~PlayerObject();

			void OnUpdate(float dt) override;
			void UpdateControls(Camera* camera);

		protected:
			bool lockOrientation;

			float yaw;
			float pitch;
			float cameraDistance;
		};
	}
}