#pragma once
#include "GameTechRenderer.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "PrefabGenerator.h"

namespace NCL {
	namespace CSC8503 {
		class Game		{
		public:
			Game();
			~Game();

			virtual void UpdateGame(float dt);

		protected:
			void Clear();

			void InitialiseAssets();

			void InitCamera();
			void UpdateKeys();

			void InitWorld();
			void InitBaseGeometry();
			void InitGauntlet1();
			void InitSlope();
			void InitGauntlet2();
	
			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();

			PrefabGenerator*	prefabGenerator;
			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*			world;

			bool useGravity;
			bool inSelectionMode;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;
			GameObject* forwardObject = nullptr;


			//Coursework Additional functionality	
			GameObject* lockedObject	= nullptr;
			Vector3 lockedOffset		= Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

		};
	}
}

