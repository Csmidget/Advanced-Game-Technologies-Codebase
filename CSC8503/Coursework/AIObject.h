#pragma once

#include "ActorObject.h"
#include "../../Common/Vector3.h"
#include "../CSC8503Common/NavigationPath.h"

namespace NCL {

	namespace CSC8503 {

		class BehaviourNode;

		class AIObject : public ActorObject {

		public:
			AIObject(Game* game, Vector3 respawnPosition,float coinHuntRange = 30.0f);
			~AIObject();

			void OnUpdate(float dt) override;
			bool SetGoal(Vector3 newGoal, float maxCost = INFINITY);

			float GetCoinHuntRange() const { return coinHuntRange; }

		protected:
			void DisplayPath();

			float behaviourUpdateCooldown;
			float coinHuntRange;

			BehaviourNode* behaviourTree;
			Quaternion orientation;

			Vector3 nextNode;
			Vector3 currentGoal;
			NavigationPath currentPath;
		};
	}
}