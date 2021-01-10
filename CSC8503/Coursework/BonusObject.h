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
				tags.push_back("bonus");
			}

			void Update(float dt) override;

			void OnCollisionBegin(GameObject* otherObject);

			bool IsPopped() const { return popped; }

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