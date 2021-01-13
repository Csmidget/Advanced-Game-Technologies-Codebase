#pragma once
#include "GameObject.h"

namespace NCL {
	namespace CSC8503 {

		class TraversableObject : public GameObject {

		public:
			TraversableObject(std::string name = "", int cost = 1) :GameObject(name) {
				traversalCost = cost;
				tags.push_back("traversable");
			}

			int TraversalCost() const { return traversalCost; }

		private:
			int traversalCost;
		};
	}
}