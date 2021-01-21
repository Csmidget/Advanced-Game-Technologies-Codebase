#pragma once

#include "BonusObject.h"

namespace NCL {
	namespace CSC8503 {

		class ScoreBonusObject : public BonusObject {

			friend class ObjectFactory;

		public:
			~ScoreBonusObject() {};

		protected:
			ScoreBonusObject(Vector3 respawnPosition, int score, float respawnDelay = 5.0f);


			void ApplyBonus(ActorObject* target) override;
			int score;
		};
	}
}