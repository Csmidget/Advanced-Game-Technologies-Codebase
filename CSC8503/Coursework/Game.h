#pragma once

#include "../CSC8503Common/PushdownMachine.h"
#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/PhysicsSystem.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "../CSC8503Common/NavigationPath.h"

namespace NCL {
	namespace CSC8503 {

		enum class CameraState { PlayerMode = 0, FreeMode = 1, SelectionMode = 2 };
		
		class PlayerObject;
		class AIObject;
		class NavigationGrid;
		class Checkpoint;
		class PrefabFactory;
		class GameTechRenderer;
		class Checkpoint;

		class Game {
		public:
			Game();
			~Game();

			virtual void UpdateGame(float dt);


			bool GetUseGravity() const { return useGravity; }
			void SetUseGravity(bool val);


			bool HasGrid() const	{ return navGrid != nullptr; }
			bool ShouldQuit() const { return quit; }

			NavigationPath			GetPath(Vector3 start, Vector3 end,float maxCost = INFINITY) const;

			GameWorld*				GetWorld() const			{ return world; }
			PlayerObject*			GetPlayerObject() const		{ return player; }
			std::vector<AIObject*>	GetOpponents() const		{ return opponents; }
			const Checkpoint*		GetGoal() const				{ return goal; }
			const PrefabFactory*	GetPrefabFactory() const	{ return prefabFactory; }

			void InitPracticeWorld();
			void InitRaceWorld(int opponents);
			void InitKatamariWorld();

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

			void InitRaceBaseGeometry();
			void InitRaceKillPlanes();
			void InitRacePlayers(int opponentCount);
			void InitRaceCheckpoints();

			void InitKatamariBaseGeometry();
			void InitKatamariKillPlanes();
			void InitKatamariPlayers(int opponentCount);

			PrefabFactory*	prefabFactory;
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

