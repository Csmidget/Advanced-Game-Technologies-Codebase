#pragma once

#include "../CSC8503Common/GameObject.h"

#include <map>

namespace NCL {
	namespace CSC8503 {

		class Constraint;
		class GameWorld;
		class ForceObject : public GameObject {

		public:

			ForceObject(GameWorld* world, MeshGeometry* mesh, TextureBase* tex, ShaderBase* shader, Vector3 position, Vector3 dimensions, Quaternion orientation, Vector3 direction, float strength);
			~ForceObject();
			void OnCollisionBegin(GameObject* otherObject) override;
			void OnCollisionEnd(GameObject* otherObject) override;

		protected: 
			float strength;
			Vector3 direction;
			std::map<GameObject*, Constraint*> activeConstraints;
			GameWorld* world;
		};
	}
}

