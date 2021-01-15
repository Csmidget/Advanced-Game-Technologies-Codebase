#pragma once
#include "GameTechRenderer.h"
#include "PrefabGenerator.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/PushdownMachine.h"
#include "../CSC8503Common/NavigationPath.h"
#include "Checkpoint.h"

namespace NCL {
	namespace CSC8503 {

		enum class CameraState { PlayerMode = 0, FreeMode = 1, SelectionMode = 2 };
		
		class PlayerObject;
		class AIObject;
		class NavigationGrid;

		class Game {
		public:
			Game();
			~Game();

			virtual void UpdateGame(float dt);
			bool ShouldQuit() { return quit; }
			GameWorld* GetWorld() { return world;	}
			PlayerObject* GetPlayerObject() { return player; }

			bool GetUseGravity() const { return useGravity; }
			void SetUseGravity(bool val);

			const Checkpoint* GetGoal() const { return goal; }

			void InitPracticeWorld();
			void InitRaceWorld(int opponents);

			NavigationPath GetPath(Vector3 start, Vector3 end,float maxCost = INFINITY);

			void SetPause(bool val) { pause = val; }
			bool IsPaused() const { return pause; }

		protected:
			void Clear();

			void InitCamera();
			void UpdateKeys();

			void InitPracticeKillPlanes();
			void InitPracticeBaseGeometry();
			void InitPracticeGauntlet1();
			void InitPracticeSlope();
			void InitPracticeGauntlet2();
			void InitPracticePlayers();
			void InitPracticeCheckpoints();

			void InitRaceKillPlanes();
			void InitRaceBaseGeometry();
			void InitRacePlayers(int opponentCount);
			void InitRaceCheckpoints();

			PrefabGenerator*	prefabGenerator;
			GameTechRenderer*	renderer;
			PhysicsSystem*		physics;
			GameWorld*			world;
			PushdownMachine*    gameStateMachine;

			bool useGravity;
			bool pause;
			bool quit;

			PlayerObject* player;
			std::vector<AIObject*> opponents;
			std::vector<Checkpoint*> checkpoints;
			std::vector<Vector3> path;
			NavigationGrid* navGrid;
			Checkpoint* goal;
		};
	}
}

