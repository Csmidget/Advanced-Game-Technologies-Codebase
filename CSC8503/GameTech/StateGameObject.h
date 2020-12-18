#pragma once

#include "..\CSC8503Common\GameObject.h"

namespace NCL {
	namespace CSC8503 {
		class StateMachine;
		class StateGameObject : public GameObject {
		public:
			StateGameObject();
			~StateGameObject();

			void MoveLeft(float dt);
			void MoveRight(float dt);

			virtual void Update(float dt) override;
		protected:
			StateMachine* stateMachine;
			float counter;
		};
	}
}