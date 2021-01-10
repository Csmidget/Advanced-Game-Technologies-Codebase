#pragma once
#pragma once
#include "../CSC8503Common/BehaviourParallel.h"

namespace NCL {
	namespace CSC8503 {

		class Game;
		class GameObject;

		class RaceAIBehaviourTree : public BehaviourParallel {

		public:
			RaceAIBehaviourTree(Game* game, GameObject* actor);

		};
	}
}

