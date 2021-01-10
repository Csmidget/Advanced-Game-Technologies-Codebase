#pragma once

#include "ActorObject.h"
#include "../../Common/Vector2.h"

namespace NCL {

	namespace CSC8503 {

		class BehaviourNode;

		class AIObject : public ActorObject {

		public:
			AIObject(Game* game, Vector3 respawnPosition);
			~AIObject();

			void OnUpdate(float dt) override;
			void OnCollisionBegin(GameObject* otherObject) override;

		protected:
			float lastCollisionTimer;
			BehaviourNode* behaviourTree;
			Quaternion orientation;
		};
	}
}