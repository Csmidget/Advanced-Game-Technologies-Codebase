#pragma once
#include "../CSC8503Common/BehaviourParallel.h"

namespace NCL {
	namespace CSC8503 {

		class Game;
		class AIObject;

		class KatamariAIBehaviourTree : public BehaviourParallel {

		public:
			KatamariAIBehaviourTree(Game* game, AIObject* actor);

		protected:
			AIObject* actor;
			Game* game;

		};
	}
}

