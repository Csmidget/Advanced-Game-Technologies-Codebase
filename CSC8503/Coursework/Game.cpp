#include "Game.h"
#include "ForceObject.h"
#include "PlayerObject.h"
#include "AIObject.h"
#include "MainMenuState.h"
#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/PositionConstraint.h"
#include "../CSC8503Common/OrientationConstraint.h"
#include "../CSC8503Common/AngularImpulseConstraint.h"
#include "../CSC8503Common/LinearImpulseConstraint.h"
#include "../CSC8503Common/CollisionDetection.h"
#include "../CSC8503Common/NavigationGrid.h"

using namespace NCL;
using namespace CSC8503;

Game::Game() {
	world		= new GameWorld();
	renderer	= new GameTechRenderer(*world);
	physics		= new PhysicsSystem(*world);
	prefabGenerator = new PrefabGenerator();
	goal = nullptr;

	useGravity		= true;
	physics->UseGravity(true);

	pause = false;
	quit = false;

	gameStateMachine = new PushdownMachine(new MainMenuState(this));
		
	Debug::SetRenderer(renderer);

	InitCamera();
}

Game::~Game()	{
	Clear();

	delete physics;
	delete renderer;
	delete world;
	delete prefabGenerator;
	delete gameStateMachine;
}

void Game::UpdateGame(float dt) {

	UpdateKeys();

	if (!pause) {
		physics->Update(dt);
		world->UpdateWorld(dt);
	}

	Vector3 playerBroadphase;
	if (player && player->GetBroadphaseAABB(playerBroadphase)) {
		for (auto cp : checkpoints) {
			if (CollisionDetection::AABBTest(player->GetTransform().GetPosition(), cp->GetPosition(), playerBroadphase, cp->GetHalfDims())) {
				//player is inside checkpoint area
				if (!player->GetCheckpoint() || cp->GetPriority() >= player->GetCheckpoint()->GetPriority())
					player->SetCheckpoint(cp);
			}
		}
	}

	if (!gameStateMachine->Update(dt) || Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		quit = true;
		return;
	}

	renderer->Update(dt);
	Debug::FlushRenderables(dt);
	renderer->Render();
}

void Game::SetUseGravity(bool val) {
	useGravity = val;
	physics->UseGravity(val);
}

void Game::UpdateKeys() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F1)) {
		InitPracticeWorld(); //We can reset the simulation at any time with F1
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2)) {
		InitCamera(); //F2 will reset the camera to a specific default place
	}

	//Running certain physics updates in a consistent order might cause some
	//bias in the calculations - the same objects might keep 'winning' the constraint
	//allowing the other one to stretch too much etc. Shuffling the order so that it
	//is random every frame can help reduce such bias.
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9)) {
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10)) {
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F7)) {
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8)) {
		world->ShuffleObjects(false);
	}

}

void Game::InitCamera() {
	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(500.0f);
	world->GetMainCamera()->SetPitch(-15.0f);
	world->GetMainCamera()->SetYaw(315.0f);
	world->GetMainCamera()->SetPosition(Vector3(-60, 40, 60));
}

void Game::Clear() {
	world->ClearAndErase();
	physics->Clear();
	path.clear();

	//As this is a game object it will be deleted by  world ClearAndErase()
	player = nullptr;

	delete navGrid;
	navGrid = nullptr;
	
	for (auto cp : checkpoints) {
		delete cp;
	}
	checkpoints.clear();

	//Opponent objects will be deleted by the world clearanderase.
	opponents.clear();

}

void Game::InitPracticeWorld() {
	Clear();

	InitPracticeKillPlanes();
	InitPracticeBaseGeometry();
	InitPracticeGauntlet1();
	InitPracticeSlope();
	InitPracticeGauntlet2();
	InitPracticePlayers();
	InitPracticeCheckpoints();
}

void Game::InitPracticeKillPlanes() {
	//Kill plane below the floor
	world->AddKillPlane(new Plane(Vector3(0, 1, 0), Vector3(0, -5, 0)));
}

void Game::InitPracticeBaseGeometry() {

	//Starting zone
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(-100, -0.5f, 100), Vector2(20, 20)));

	//First Gauntlet
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(-100, -0.5f, 0.0f), Vector2(10, 80)));

	//Checkpoint1
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(-100, -0.5f, -100.0f), Vector2(20, 20)));

	//Slope
	world->AddGameObject(prefabGenerator->CreateOrientedFloor(Vector3(0.0f, 50.5f, -100.0f), Quaternion::EulerAnglesToQuaternion(0, 0, 32.5), Vector2(95.15, 10)));

	//Checkpoint2
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(100.0f, 101.5f, -100.0f), Vector2(20, 20)));

	//Second Gauntlet
	//world->AddGameObject(prefabGenerator->CreateFloor(Vector3(100.0f, 86.35f, 0.0f), Vector2(12.5, 75)));

	//Goal
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(100.0f, 101.5f, 100.0f), Vector2(20, 20)));

}

void Game::InitPracticeGauntlet1() {

	prefabGenerator->AddSpinningBlock(world, Vector3(-100, 3.0f, -30.0f), Vector3(0, 1, 0), -10000.0f);
	prefabGenerator->AddSpinningBlock(world, Vector3(-100, 3.0f, 0.0f), Vector3(0, 1, 0), 10000.0f);
	prefabGenerator->AddSpinningBlock(world, Vector3(-100, 3.0f, 30.0f), Vector3(0, 1, 0), -10000.0f);

	//Coins
	prefabGenerator->AddScoreBonus(world, Vector3(-100, 1.0f, 60.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(-100, 1.0f, 45.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(-105, 1.0f, 30.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(-100, 1.0f, 15.0f));
	prefabGenerator->AddScoreBonus(world, Vector3( -95, 1.0f, 0.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(-100, 1.0f, -15.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(-105, 1.0f, -30.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(-100, 1.0f, -45.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(-100, 1.0f, -60.0f));
}

void Game::InitPracticeSlope() {

	//Falling objects
	for (int i = 1; i <= 4; ++i) { 
		float height = i * 150;

		world->AddGameObject(prefabGenerator->CreateCapsule(Vector3( 70.0f, height, -105.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true));
		world->AddGameObject(prefabGenerator->CreateCapsule(Vector3( 70.0f, height, -100.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true));
		world->AddGameObject(prefabGenerator->CreateCapsule(Vector3( 70.0f, height, - 95.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true));

		world->AddGameObject(prefabGenerator->CreateSphere(Vector3(70.0f, height + 50, -105.0f), 1.0f, 0.2f, true));
		world->AddGameObject(prefabGenerator->CreateSphere(Vector3(70.0f, height + 50, -100.0f), 1.0f, 0.2f, true));
		world->AddGameObject(prefabGenerator->CreateSphere(Vector3(70.0f, height + 50, - 95.0f), 1.0f, 0.2f, true));

		world->AddGameObject(prefabGenerator->CreateOBBCube(Vector3(70.0f, height + 100, -107.0f), Quaternion(), Vector3(1, 1, 1), 0.1f, true));
		world->AddGameObject(prefabGenerator->CreateOBBCube(Vector3(70.0f, height + 100, -100.0f), Quaternion(), Vector3(1, 1, 1), 0.1f, true));
		world->AddGameObject(prefabGenerator->CreateOBBCube(Vector3(70.0f, height + 100, - 93.0f), Quaternion(), Vector3(1, 1, 1), 0.1f, true));
	}

	//Coins
	prefabGenerator->AddScoreBonus(world, Vector3(-75.0f, 4.5f, -100.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(-50.0f, 20.5f, -100.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(-25.0f, 36.25f, -100.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(0.0f, 52.0f, -100.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(25.0f, 67.75, -100.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(50.0f, 83.5f, -100.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(75.0f, 99.5f, -100.0f));
}

void Game::InitPracticeGauntlet2() {
	float y = 101.5f;

	world->AddGameObject(prefabGenerator->CreateSlipperyFloor(Vector3(115.0f, y, -60.0f), Quaternion(), Vector2(5.0f, 20.0f)));
	prefabGenerator->AddBouncePad(world, Vector3(115.0f, y - 1.1f, -38.25f), Matrix4::Rotation(30,Vector3(1,0,0)), 5.2f, Vector2(4.95f, 2.25f));

	world->AddGameObject(prefabGenerator->CreateSlipperyFloor(Vector3(100.0f, y, -45.0f), Quaternion(), Vector2(10.0f, 1.5f)));

	prefabGenerator->AddTreadmill(world, Vector3(85.0f, y, -25.0f), Quaternion(), 0.1f, Vector2(5.0f, 25.0f));

	world->AddGameObject(prefabGenerator->CreateSlipperyFloor(Vector3(103.0f, y, -5.0f), Quaternion(), Vector2(13.0f, 1.0f)));

//	prefabGenerator->AddPendulum(world, Vector3(106.25f, 96.35f, 1.25f), 8.0f, Vector3(0, 0, 30));

	world->AddGameObject(prefabGenerator->CreateSlipperyFloor(Vector3(115.5f, y, 3.0f), Quaternion(), Vector2(0.5f,7.0f)));

	prefabGenerator->AddTreadmill(world, Vector3(105.0f, y, 15.0f), Matrix4::Rotation(90,Vector3(0,1,0)), 0.25f, Vector2(5.0f, 15.0f));
	prefabGenerator->AddTreadmill(world, Vector3(85.0f, y, 25.0f), Matrix4::Rotation(180, Vector3(0, 1, 0)), 0.25f, Vector2(5.0f, 15.0f));
	prefabGenerator->AddTreadmill(world, Vector3(95.0f, y, 45.0f), Matrix4::Rotation(-90, Vector3(0, 1, 0)), 0.25f, Vector2(5.0f, 15.0f));
	prefabGenerator->AddTreadmill(world, Vector3(115.0f, y, 35.0f), Quaternion(), 0.25f, Vector2(5.0f, 15.0f));

	world->AddGameObject(prefabGenerator->CreateSphere(Vector3(115, y + 2, 35.0f), 1.0f, 0.6f, true));

	world->AddGameObject(prefabGenerator->CreateSlipperyFloor(Vector3(100.0f, y, 65.0f), Quaternion(), Vector2(2.0f, 15.0f)));
	
	float coinY = y + 1.5f;
	//Coins
	prefabGenerator->AddScoreBonus(world, Vector3(87.5f, coinY, -49.5f));
	prefabGenerator->AddScoreBonus(world, Vector3(82.5f, coinY, -49.5f));

	prefabGenerator->AddScoreBonus(world, Vector3(85.0f, coinY, -35.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(85.0f, coinY, -25.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(85.0f, coinY, -15.0f));

	prefabGenerator->AddScoreBonus(world, Vector3(115.5f, y + 7.0f, -25.0f));

	prefabGenerator->AddScoreBonus(world, Vector3(105.0f, coinY, 15.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(85.0f , coinY, 25.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(95.0f , coinY, 45.0f));
	prefabGenerator->AddScoreBonus(world, Vector3(115.0f, coinY, 35.0f));
}

void Game::InitPracticePlayers() {
	player = prefabGenerator->AddPlayer(this, Vector3(-100, 5, 100));
	player->AddScore(1000.0f);
}

void Game::InitPracticeCheckpoints() {
	checkpoints.push_back(new Checkpoint(Vector3(-100, 10, -100), Vector3(20, 10, 20), 1));
	checkpoints.push_back(new Checkpoint(Vector3(100, 111, -100), Vector3(20, 10, 20), 2));

	goal = new Checkpoint(Vector3( 100, 111, 100), Vector3(20, 10, 20), 2);
}

void Game::InitRaceWorld(int players) {
	Clear();

	InitRaceBaseGeometry();
	InitRaceKillPlanes();
	InitRaceCheckpoints();
	InitRacePlayers(players);

	//navGrid = new NavigationGrid("RaceGrid.txt", Vector3(125, 0, 125));
	navGrid = new NavigationGrid(world->GetStaticObjectTree(), Vector3(0, 0, 0), 1000, Vector2(200, 200), 2.5f);

	NavigationPath outPath;
	navGrid->FindPath(Vector3(-100, 0, 100), Vector3(-100, 0, -100), outPath);

	Vector3 pos;
	while (outPath.PopWaypoint(pos)) {
		path.push_back(pos);
	};
}

void Game::InitRaceBaseGeometry() {

	//Starting zone
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(-100, -0.5f, 100), Vector2(20, 20)));

	//First Gauntlet
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(-100, -0.5f, 0.0f), Vector2(15, 80)));
	world->AddGameObject(prefabGenerator->CreateAABBCube(Vector3(-100, 2, 0), Vector3(5, 2, 2), 0.0f, true));
	world->AddGameObject(prefabGenerator->CreateAABBCube(Vector3(-107.5, 2, 50), Vector3(5, 2, 2), 0.0f, true));

	world->AddGameObject(prefabGenerator->CreateAABBCube(Vector3(-100  , 2, -57.5), Vector3(6.25, 2, 1.25), 0.0f, true));
	world->AddGameObject(prefabGenerator->CreateAABBCube(Vector3(-105, 2, -65), Vector3(1.25, 2, 6.25), 0.0f, true));
	world->AddGameObject(prefabGenerator->CreateAABBCube(Vector3(- 95  , 2, -65), Vector3(1.25, 2, 6.25), 0.0f, true));

	//Checkpoint1
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(-100, -0.5f, -100.0f), Vector2(20, 20)));
}

void Game::InitRaceKillPlanes() {
	//Kill plane below the floor
	world->AddKillPlane(new Plane(Vector3(0, 1, 0), Vector3(0, -5, 0)));
}


void Game::InitRaceCheckpoints() {
	goal = new Checkpoint(Vector3(-100, 10, -100), Vector3(20, 10, 20), 1);

}

void Game::InitRacePlayers(int opponentCount) {
	player = prefabGenerator->AddPlayer(this, Vector3(-95, 5, 100));
	player->AddScore(100000.0f);

	opponents.push_back(prefabGenerator->AddAI(this, Vector3(-105, 5, 100)));

	prefabGenerator->AddScoreBonus(world, Vector3(-100, 1.0f, 100), 10.0f);
	prefabGenerator->AddScoreBonus(world, Vector3(-110, 1.0f, 45.0f), 10.0f);
	prefabGenerator->AddScoreBonus(world, Vector3(-90, 1.0f, 0.0f), 10.0f);
	prefabGenerator->AddScoreBonus(world, Vector3(-100.0, 1.0f, -60.0f),10.0f);
}

NavigationPath Game::GetPath(Vector3 start, Vector3 end, float maxCost) {
	NavigationPath generatedPath;
	navGrid->FindPath(start, end, generatedPath,maxCost);
	return generatedPath;
}
