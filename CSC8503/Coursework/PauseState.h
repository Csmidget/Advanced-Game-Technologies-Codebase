#pragma once
#include "GameState.h"

namespace NCL {
	namespace CSC8503 {

		class Camera* camera;

		class PauseState : public GameState {

		public:
			PauseState(Game* game);

		protected:
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;

			const Camera* camera;
		};

	}
}

