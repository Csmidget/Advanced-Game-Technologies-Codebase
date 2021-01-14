#pragma once
#include "GameObject.h"

namespace NCL {
	namespace CSC8503 {

		class TraversableObject : public GameObject {

		public:
			TraversableObject(std::string name = "", char type = '.') :GameObject(name) {
				traversalType = type;
				tags.push_back("traversable");
			}

			char TraversalType() const { return traversalType; }

		private:
			char traversalType;
		};
	}
}