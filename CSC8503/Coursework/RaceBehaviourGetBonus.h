#pragma once
#include "../CSC8503Common/BehaviourSequence.h"

namespace NCL {
	namespace CSC8503 {

		class Game;
		class GameObject;
		class BonusObject;

		class RaceBehaviourGetBonus : public BehaviourSequence {

		public:
			RaceBehaviourGetBonus(Game* game, GameObject* actor);

		protected:
			Game* game;
			GameObject* actor;
			BonusObject* bonusTarget;
		};
	}
}
