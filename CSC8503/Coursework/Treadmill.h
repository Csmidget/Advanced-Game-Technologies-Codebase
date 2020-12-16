#pragma once

#include "../CSC8503Common/GameObject.h"

#include <map>

namespace NCL {
	namespace CSC8503 {

		class Constraint;
		class GameWorld;
		class Treadmill : public GameObject {

		public:

			Treadmill(GameWorld* world, MeshGeometry* mesh, TextureBase* tex, ShaderBase* shader, Vector3 position, Vector3 dimensions, Vector3 direction);
			~Treadmill();
			void OnCollisionBegin(GameObject* otherObject) override;
			void OnCollisionEnd(GameObject* otherObject) override;

		protected: 
			Vector3 direction;
			std::map<GameObject*, Constraint*> activeConstraints;
			GameWorld* world;
		};
	}
}

