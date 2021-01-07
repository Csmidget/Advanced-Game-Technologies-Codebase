#pragma once
#include "GameState.h"

namespace NCL {

	class Camera;

	namespace CSC8503 {
		class PracticeState : public GameState {

		public:
			PracticeState(Game* game);

		protected:
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;

			Camera* camera;

			float yaw;
			float pitch;
			float cameraDistance;
			float scoreTracker;
		};

	}
}

