#pragma once
#include "GameState.h"
#include "BoidSwarm.h";

namespace NCL {

	class Camera;

	namespace CSC8503 {

		class BoidSwarm;

		class KatamariState : public GameState {

		public:
			KatamariState(Game* game);
			~KatamariState();

		protected:

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;

			Camera* camera;

			BoidSwarm* boidSwarm;
		};

	}
}

