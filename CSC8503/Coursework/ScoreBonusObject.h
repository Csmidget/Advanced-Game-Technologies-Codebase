#pragma once

#include "BonusObject.h"

namespace NCL {
	namespace CSC8503 {

		class ScoreBonusObject : public BonusObject {
		public:
			ScoreBonusObject(Vector3 respawnPosition, int score,float respawnDelay = 5.0f);
			~ScoreBonusObject() {};

		protected:
			void ApplyBonus(ActorObject* target) override;
			int score;
		};
	}
}