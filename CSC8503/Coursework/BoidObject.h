#pragma once
#include "RespawningObject.h"

#include "../../Common/Vector2.h"

namespace NCL {

	namespace CSC8503 {
		class BoidSwarm;
		class Game;

		class BoidObject : public RespawningObject {

			friend class PrefabFactory;

		public:
			~BoidObject();

			void Update(float dt) override;
			void OnCollisionBegin(GameObject* otherObject) override;

		protected:
			BoidObject(Game* game, BoidSwarm** swarm, Vector3 respawnPosition);

			Game* game;
			BoidSwarm** swarm;
		};
	}
}