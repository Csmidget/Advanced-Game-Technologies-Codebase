#pragma once

#include "../CSC8503Common/GameObject.h"

namespace NCL {
	namespace CSC8503 {

		class RespawningObject : public GameObject {
			
			friend class ObjectFactory;

		protected:
			RespawningObject(Vector3 spawnPosition, string name, bool randomizeOrientation = false);

		public:
			virtual void Respawn();
			virtual void OnRespawn() {};
			virtual void Update(float dt) override;
			virtual void ObjectSpecificDebugInfo(int& currLine, float lineSpacing) const override;

			void SetRespawnPosition(Vector3 val) { spawnPosition = val; }

		private:
			void OnKill() override;
			Vector3 spawnPosition;
			bool randomizeOrientation;

			Vector3 prevPos;
			float respawnTimer;
		};
	}
}