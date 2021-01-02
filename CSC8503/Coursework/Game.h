#pragma once
#include "GameTechRenderer.h"
#include "PrefabGenerator.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/PushdownMachine.h"

namespace NCL {
	namespace CSC8503 {

		enum class CameraState { PlayerMode = 0, FreeMode = 1, SelectionMode = 2 };
		class PlayerObject;

		class Game {
		public:
			Game();
			~Game();

			virtual void UpdateGame(float dt);
			bool ShouldQuit() { return quit; }
			const GameWorld* GetWorld() { return world;	}
			PlayerObject* GetPlayerObject() { return player; }

			void ResetWorld();

			void SetPause(bool val) { pause = val; }

		protected:
			void Clear();

			void InitCamera();
			void UpdateKeys();

			void InitWorld();
			void InitKillPlanes();
			void InitBaseGeometry();
			void InitGauntlet1();
			void InitSlope();
			void InitGauntlet2();
			void InitPlayers();
	
			bool SelectObject();
			void MoveSelectedObject();
			void DebugObjectMovement();
			void LockedObjectMovement();

			PrefabGenerator*	prefabGenerator;
			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*			world;
			PushdownMachine*    gameStateMachine;

			bool useGravity;
			bool pause;
			bool quit;

			float		forceMagnitude;

			GameObject* selectionObject = nullptr;
			GameObject* forwardObject = nullptr;

			CameraState cameraState;

			PlayerObject* player;

			//Coursework Additional functionality	
			GameObject* lockedObject	= nullptr;
			Vector3 lockedOffset		= Vector3(0, 14, 20);
			void LockCameraToObject(GameObject* o) {
				lockedObject = o;
			}

		};
	}
}

