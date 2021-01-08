#pragma once
#include "GameTechRenderer.h"
#include "PrefabGenerator.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/PushdownMachine.h"
#include "Checkpoint.h"

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

			bool GetUseGravity() const { return useGravity; }
			void SetUseGravity(bool val);

			const Checkpoint* GetGoal() const { return goal; }

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
			void InitCheckpoints();

			PrefabGenerator*	prefabGenerator;
			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*			world;
			PushdownMachine*    gameStateMachine;

			bool useGravity;
			bool pause;
			bool quit;

			PlayerObject* player;
			std::vector<Checkpoint*> checkpoints;
			Checkpoint* goal;
		};
	}
}

