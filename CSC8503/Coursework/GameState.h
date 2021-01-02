#pragma once
#include "../CSC8503Common/PushdownState.h" 

namespace NCL {
	namespace CSC8503 {
		class Game;

		class GameState : public PushdownState {

		public:
			GameState(Game* game) { this->game = game; }

		protected:
			Game* game;
		};
	}
}