#pragma once

#include "ActorObject.h"
#include "../../Common/Vector3.h"
#include "../CSC8503Common/NavigationPath.h"

namespace NCL {

	namespace CSC8503 {

		class BehaviourNode;

		class AIObject : public ActorObject {

		public:
			AIObject(Game* game, Vector3 respawnPosition);
			~AIObject();

			void OnUpdate(float dt) override;
			bool SetGoal(Vector3 newGoal, float maxCost = INFINITY);


		protected:
			void DisplayPath();

			float setGoalCooldown;

			BehaviourNode* behaviourTree;
			Quaternion orientation;

			Vector3 nextNode;
			Vector3 currentGoal;
			NavigationPath currentPath;
		};
	}
}