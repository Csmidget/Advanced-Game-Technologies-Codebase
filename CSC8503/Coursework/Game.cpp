#include "Game.h"
#include "ForceObject.h"
#include "PlayerObject.h"
#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/PositionConstraint.h"
#include "../CSC8503Common/OrientationConstraint.h"
#include "../CSC8503Common/AngularImpulseConstraint.h"
#include "MainMenuState.h"


using namespace NCL;
using namespace CSC8503;

Game::Game() {
	world		= new GameWorld();
	renderer	= new GameTechRenderer(*world);
	physics		= new PhysicsSystem(*world);
	prefabGenerator = new PrefabGenerator();

	useGravity		= true;
	physics->UseGravity(true);

	pause = false;
	quit = false;

	gameStateMachine = new PushdownMachine(new MainMenuState(this));
		
	Debug::SetRenderer(renderer);

	InitCamera();
}

Game::~Game()	{
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
	}

	if (!gameStateMachine->Update(dt) || Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		quit = true;
		return;
	}

	if (!pause) {
		world->UpdateWorld(dt);
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
		InitWorld(); //We can reset the simulation at any time with F1
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

	//As this is a game object it will be deleted by  world ClearAndErase()
	player = nullptr;
}

void Game::ResetWorld() {
	InitWorld();
}

void Game::InitWorld() {
	Clear();

	InitKillPlanes();
	InitBaseGeometry();
	InitGauntlet1();
	InitSlope();
	InitGauntlet2();
	InitPlayers();
}

void Game::InitKillPlanes() {
	//Kill plane below the floor
	world->AddKillPlane(new Plane(Vector3(0, 1, 0), Vector3(0, -5, 0)));
}

void Game::InitBaseGeometry() {

	//Starting zone
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(-100, -0.5f, 100), Vector2(25, 25)));

	//First Gauntlet
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(-100, -0.5f, 0.0f), Vector2(12.5, 75)));

	//Checkpoint1
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(-100, -0.5f, -100.0f), Vector2(25, 25)));

	//Slope
	world->AddGameObject(prefabGenerator->CreateOrientedFloor(Vector3(0.0f, 43.0f, -100.0f), Quaternion::EulerAnglesToQuaternion(0, 0, 30), Vector2(87, 12.5)));

	//Checkpoint2
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(100.0f, 86.35f, -100.0f), Vector2(25, 25)));

	//Second Gauntlet
	//world->AddGameObject(prefabGenerator->CreateFloor(Vector3(100.0f, 86.35f, 0.0f), Vector2(12.5, 75)));

	//Goal
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(100.0f, 86.35f, 100.0f), Vector2(25, 25)));

}

void Game::InitGauntlet1() {

	prefabGenerator->AddSpinningBlock(world, Vector3(-100, 3.0f, -30.0f), Vector3(0, 1, 0), -100.0f);
	prefabGenerator->AddSpinningBlock(world, Vector3(-100, 3.0f, 0.0f), Vector3(0, 1, 0), 100.0f);
	prefabGenerator->AddSpinningBlock(world, Vector3(-100, 3.0f, 30.0f), Vector3(0, 1, 0), -100.0f);
}

void Game::InitSlope() {

	for (int i = 1; i <= 4; ++i) { 
		float height = i * 150;

		world->AddGameObject(prefabGenerator->CreateCapsule(Vector3( 70.0f, height, -110.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true));
		world->AddGameObject(prefabGenerator->CreateCapsule(Vector3( 70.0f, height, -105.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true));
		world->AddGameObject(prefabGenerator->CreateCapsule(Vector3( 70.0f, height, -100.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true));
		world->AddGameObject(prefabGenerator->CreateCapsule(Vector3( 70.0f, height, - 95.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true));
		world->AddGameObject(prefabGenerator->CreateCapsule(Vector3( 70.0f, height, - 90.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true));

		world->AddGameObject(prefabGenerator->CreateSphere(Vector3(70.0f, height + 50, -110.0f), 1.0f, 0.2f, true));
		world->AddGameObject(prefabGenerator->CreateSphere(Vector3(70.0f, height + 50, -105.0f), 1.0f, 0.2f, true));
		world->AddGameObject(prefabGenerator->CreateSphere(Vector3(70.0f, height + 50, -100.0f), 1.0f, 0.2f, true));
		world->AddGameObject(prefabGenerator->CreateSphere(Vector3(70.0f, height + 50, - 95.0f), 1.0f, 0.2f, true));
		world->AddGameObject(prefabGenerator->CreateSphere(Vector3(70.0f, height + 50, - 90.0f), 1.0f, 0.2f, true));

		world->AddGameObject(prefabGenerator->CreateOBBCube(Vector3(70.0f, height + 100, -107.0f), Quaternion(), Vector3(1, 1, 1), 0.1f, true));
		world->AddGameObject(prefabGenerator->CreateOBBCube(Vector3(70.0f, height + 100, -100.0f), Quaternion(), Vector3(1, 1, 1), 0.1f, true));
		world->AddGameObject(prefabGenerator->CreateOBBCube(Vector3(70.0f, height + 100, - 93.0f), Quaternion(), Vector3(1, 1, 1), 0.1f, true));
	}
}

void Game::InitGauntlet2() {
	world->AddGameObject(prefabGenerator->CreateSphere(Vector3(100, 90, -100), 2.0f, 10.0f));
	world->AddGameObject(prefabGenerator->CreateSlipperyFloor(Vector3(118.75f, 86.35f, -55.0f), Quaternion(), Vector2(6.25f, 20.0f)));
	world->AddGameObject(prefabGenerator->CreateSlipperyFloor(Vector3(100.0f, 86.35f, -41.25f), Quaternion(), Vector2(12.5f, 6.25f)));
	world->AddGameObject(prefabGenerator->AddTreadmill(world, Vector3(81.25f, 86.75f, -26.25f), Quaternion(), 10.0f, Vector2(6.25f, 21.25f)));
}

void Game::InitPlayers() {
	player = prefabGenerator->AddPlayer(world, Vector3(-100, 5, 100));
}

//GameObject* Game::AddPlayerToWorld(const Vector3& position) {
//	float meshSize = 3.0f;
//	float inverseMass = 0.5f;
//
//	GameObject* character = new GameObject("player");
//
//	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.85f, 0.3f) * meshSize);
//
//	character->SetBoundingVolume((CollisionVolume*)volume);
//
//	character->GetTransform()
//		.SetScale(Vector3(meshSize, meshSize, meshSize))
//		.SetPosition(position);
//
//	if (rand() % 2) {
//		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshA, nullptr, basicShader));
//	}
//	else {
//		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshB, nullptr, basicShader));
//	}
//	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));
//
//	character->GetPhysicsObject()->SetInverseMass(inverseMass);
//	character->GetPhysicsObject()->InitSphereInertia();
//
//	world->AddGameObject(character);
//
//	//lockedObject = character;
//
//	return character;
//}

//GameObject* Game::AddEnemyToWorld(const Vector3& position) {
//	float meshSize		= 3.0f;
//	float inverseMass	= 0.5f;
//
//	GameObject* character = new GameObject("enemy");
//
//	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
//	character->SetBoundingVolume((CollisionVolume*)volume);
//
//	character->GetTransform()
//		.SetScale(Vector3(meshSize, meshSize, meshSize))
//		.SetPosition(position);
//
//	character->SetRenderObject(new RenderObject(&character->GetTransform(), enemyMesh, nullptr, basicShader));
//	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));
//
//	character->GetPhysicsObject()->SetInverseMass(inverseMass);
//	character->GetPhysicsObject()->InitSphereInertia();
//
//	world->AddGameObject(character);
//
//	return character;
//}

//GameObject* Game::AddBonusToWorld(const Vector3& position) {
//	GameObject* apple = new GameObject();
//
//	SphereVolume* volume = new SphereVolume(0.25f);
//	apple->SetBoundingVolume((CollisionVolume*)volume);
//	apple->GetTransform()
//		.SetScale(Vector3(0.25, 0.25, 0.25))
//		.SetPosition(position);
//
//	apple->SetRenderObject(new RenderObject(&apple->GetTransform(), bonusMesh, nullptr, basicShader));
//	apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));
//
//	apple->GetPhysicsObject()->SetInverseMass(1.0f);
//	apple->GetPhysicsObject()->InitSphereInertia();
//
//	world->AddGameObject(apple);
//
//	return apple;
//}
