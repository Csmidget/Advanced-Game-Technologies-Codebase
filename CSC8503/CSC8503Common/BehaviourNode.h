#pragma once

#include <string>

namespace NCL {
	namespace CSC8503 {

		enum class BehaviourState {
			Initialise,
			Failure,
			Success,
			Ongoing
		};

		class BehaviourNode {
		public:
			BehaviourNode(const std::string& nodeName) {
				currentState = BehaviourState::Initialise;
				name = nodeName;
			}
			virtual ~BehaviourNode() {};
			virtual BehaviourState Execute(float dt) = 0;

			virtual void Reset() { currentState = BehaviourState::Initialise; }

		protected:
			BehaviourState currentState;
			std::string name;
		};
	}
}

