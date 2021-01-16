#pragma once

#include "ActorObject.h"
#include "../../Common/Vector3.h"
#include "../CSC8503Common/NavigationPath.h"

namespace NCL {

	namespace CSC8503 {

		class BehaviourNode;

		class AIObject : public ActorObject {

		public:
			AIObject(Game* game, Vector3 respawnPosition, std::string name = "enemy",float coinHuntRange = 30.0f, float angerThreshold = 3.0f, float strength = 2.0f);
			~AIObject();

			void OnUpdate(float dt) override;
			bool SetGoal(Vector3 newGoal, float maxCost = INFINITY, bool force = false);

			float GetCoinHuntRange() const { return coinHuntRange; }

			void ObjectSpecificDebugInfo(int& currLine, float lineSpacing) override;

			void SetCurrentState(std::string str) { currentState = str; }

			void OnCollisionBegin(GameObject* otherObject) override;
			 
			bool IsAngry() { return currentAnger >= angerThreshold; }

			void OnRespawn() override;

			bool IsAsleep() const { return asleep; }
			void SetAsleep(bool val) { asleep = val; }


		protected:
			void DisplayPath();

			bool asleep;

			float angerThreshold;
			float currentAnger;
			float strength;

			float behaviourUpdateCooldown;
			float coinHuntRange;
			std::string currentState;

			BehaviourNode* behaviourTree;
			Quaternion orientation;

			Vector3 nextNode;
			Vector3 currentGoal;
			NavigationPath currentPath;
		};
	}
}