#pragma once

#include "../CSC8503Common/GameObject.h"


namespace NCL {

	namespace CSC8503 {

		class RespawningObject : public GameObject {

		public:
			RespawningObject(Vector3 spawnPosition,bool randomizeOrientation = false, string name = "");
			~RespawningObject();

			void OnKill() override;
			void Update(float dt) override;

		protected:
			void Respawn();

			Vector3 spawnPosition;
			bool randomizeOrientation;

			Vector3 prevPos;
			float respawnTimer;
		};
	}
}