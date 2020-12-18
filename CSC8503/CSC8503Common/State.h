#pragma once

namespace NCL {
	namespace CSC8503 {
		class State		{
		public:
			State() {}
			virtual ~State() {}
			virtual void Update(float dt) = 0; //Pure virtual base class
		};

		typedef void(*StateFunc)(float, void*);

		class GenericState : public State		{
		public:
			GenericState(StateFunc someFunc, void* someData) {
				func		= someFunc;
				funcData	= someData;
			}
			virtual void Update(float dt) {
				if (funcData != nullptr) {
					func(dt, funcData);
				}
			}
		protected:
			StateFunc func;
			void* funcData;
		};

		class SuperState : public State {
		protected:

		};
	}
}