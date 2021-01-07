#pragma once

namespace NCL {
	namespace CSC8503 {

		class ActorObject;

		class Powerup {

			void Update(ActorObject* target, float dt) {
				timeRemaining -= dt;
			}

			float timeRemaining;
		};

	}
}