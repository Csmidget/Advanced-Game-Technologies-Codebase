#pragma once
#include "RespawningObject.h"
#include "../../Common/Vector2.h"

namespace NCL {

	namespace CSC8503 {
		class BoidSwarm;
		class Game;

		class BoidObject : public RespawningObject {

		public:
			BoidObject(Game* game, BoidSwarm** swarm, Vector3 respawnPosition);
			~BoidObject();

			void Update(float dt) override;
			void OnCollisionBegin(GameObject* otherObject) override;

		protected:
			Game* game;
			BoidSwarm** swarm;
		};
	}
}