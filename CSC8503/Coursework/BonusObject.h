#pragma once

#include "RespawningObject.h"

namespace NCL {
	namespace CSC8503 {
		
		class GameWorld;
		class ActorObject;

		class BonusObject : public RespawningObject {

		public:
			BonusObject(Vector3 respawnPosition, float respawnDelay = 5.0f) : RespawningObject(respawnPosition, "bonus") {
				popped = false;
				respawnTimer = respawnDelay;
				this->respawnDelay = respawnDelay;
			}

			void Update(float dt) override;

			void OnCollisionBegin(GameObject* otherObject);

		protected:
			virtual void ApplyBonus(ActorObject* target) = 0;

		private:
			bool popped;
			float respawnDelay;
			float respawnTimer;
			Vector3 savedScale;
		};
	}
}