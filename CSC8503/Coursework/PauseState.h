#pragma once
#include "GameState.h"

namespace NCL {

	class Camera;

	namespace CSC8503 {

		class GameWorld;
		class GameObject;

		class PauseState : public GameState {

		public:
			PauseState(Game* game);
			~PauseState();

		protected:
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;

			Camera* camera;
			const GameWorld* world;

			bool selectionMode;
			GameObject* selectionObject;

		};

	}
}

