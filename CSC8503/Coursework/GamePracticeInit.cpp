#include "Game.h"
#include "Checkpoint.h"


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
	prefabGenerator->CreateFloor(world, Vector3(-100, -0.5f, 100), Vector2(20, 20));

	//First Gauntlet
	prefabGenerator->CreateFloor(world, Vector3(-100, -0.5f, 0.0f), Vector2(10, 80));

	//Checkpoint1
	prefabGenerator->CreateFloor(world, Vector3(-100, -0.5f, -100.0f), Vector2(20, 20));

	//Slope
	prefabGenerator->CreateOrientedFloor(world, Vector3(0.0f, 50.5f, -100.0f), Quaternion::EulerAnglesToQuaternion(0, 0, 32.5), Vector2(95.15, 10));

	//Checkpoint2
	prefabGenerator->CreateFloor(world, Vector3(100.0f, 101.5f, -100.0f), Vector2(20, 20));

	//Second Gauntlet
	//world->AddGameObject(prefabGenerator->CreateFloor(Vector3(100.0f, 86.35f, 0.0f), Vector2(12.5, 75)));

	//Goal
	prefabGenerator->CreateFloor(world, Vector3(100.0f, 101.5f, 100.0f), Vector2(20, 20));

}

void Game::InitPracticeGauntlet1() {

	prefabGenerator->CreateSpinningBlock(world, Vector3(-100, 3.0f, -30.0f), Vector3(0, 1, 0), -10000.0f);
	prefabGenerator->CreateSpinningBlock(world, Vector3(-100, 3.0f, 0.0f), Vector3(0, 1, 0), 10000.0f);
	prefabGenerator->CreateSpinningBlock(world, Vector3(-100, 3.0f, 30.0f), Vector3(0, 1, 0), -10000.0f);

	//Coins
	prefabGenerator->CreateScoreBonus(world, Vector3(-100, 1.0f, 60.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(-100, 1.0f, 45.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(-105, 1.0f, 30.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(-100, 1.0f, 15.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(-95, 1.0f, 0.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(-100, 1.0f, -15.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(-105, 1.0f, -30.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(-100, 1.0f, -45.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(-100, 1.0f, -60.0f));
}

void Game::InitPracticeSlope() {

	//Falling objects
	for (int i = 1; i <= 4; ++i) {
		float height = i * 150;

		prefabGenerator->CreateCapsule(world, Vector3(70.0f, height, -105.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true);
		prefabGenerator->CreateCapsule(world, Vector3(70.0f, height, -100.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true);
		prefabGenerator->CreateCapsule(world, Vector3(70.0f, height, -95.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true);

		prefabGenerator->CreateSphere(world, Vector3(70.0f, height + 50, -105.0f), 1.0f, 0.2f, true);
		prefabGenerator->CreateSphere(world, Vector3(70.0f, height + 50, -100.0f), 1.0f, 0.2f, true);
		prefabGenerator->CreateSphere(world, Vector3(70.0f, height + 50, -95.0f), 1.0f, 0.2f, true);

		prefabGenerator->CreateOBBCube(world, Vector3(70.0f, height + 100, -107.0f), Quaternion(), Vector3(1, 1, 1), 0.1f, true);
		prefabGenerator->CreateOBBCube(world, Vector3(70.0f, height + 100, -100.0f), Quaternion(), Vector3(1, 1, 1), 0.1f, true);
		prefabGenerator->CreateOBBCube(world, Vector3(70.0f, height + 100, -93.0f), Quaternion(), Vector3(1, 1, 1), 0.1f, true);
	}

	//Coins
	prefabGenerator->CreateScoreBonus(world, Vector3(-75.0f, 4.5f, -100.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(-50.0f, 20.5f, -100.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(-25.0f, 36.25f, -100.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(0.0f, 52.0f, -100.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(25.0f, 67.75, -100.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(50.0f, 83.5f, -100.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(75.0f, 99.5f, -100.0f));
}

void Game::InitPracticeGauntlet2() {
	float y = 101.5f;

	prefabGenerator->CreateSlipperyFloor(world, Vector3(115.0f, y, -60.0f), Quaternion(), Vector2(5.0f, 20.0f));
	prefabGenerator->CreateBouncePad(world, Vector3(115.0f, y - 1.1f, -38.25f), Matrix4::Rotation(30, Vector3(1, 0, 0)), 5.2f, Vector2(4.95f, 2.25f));

	prefabGenerator->CreateSlipperyFloor(world, Vector3(100.0f, y, -45.0f), Quaternion(), Vector2(10.0f, 1.5f));

	prefabGenerator->CreateTreadmill(world, Vector3(85.0f, y, -25.0f), Quaternion(), 4.0f, Vector2(5.0f, 25.0f));

	prefabGenerator->CreateSlipperyFloor(world, Vector3(103.0f, y, -5.0f), Quaternion(), Vector2(13.0f, 1.0f));

	//	prefabGenerator->AddPendulum(world, Vector3(106.25f, 96.35f, 1.25f), 8.0f, Vector3(0, 0, 30));

	prefabGenerator->CreateSlipperyFloor(world, Vector3(115.5f, y, 3.0f), Quaternion(), Vector2(0.5f, 7.0f));

	prefabGenerator->CreateTreadmill(world, Vector3(105.0f, y, 15.0f), Matrix4::Rotation(90, Vector3(0, 1, 0)), 4.0f, Vector2(5.0f, 15.0f));
	prefabGenerator->CreateTreadmill(world, Vector3(85.0f, y, 25.0f), Matrix4::Rotation(180, Vector3(0, 1, 0)), 4.0f, Vector2(5.0f, 15.0f));
	prefabGenerator->CreateTreadmill(world, Vector3(95.0f, y, 45.0f), Matrix4::Rotation(-90, Vector3(0, 1, 0)), 4.0f, Vector2(5.0f, 15.0f));
	prefabGenerator->CreateTreadmill(world, Vector3(115.0f, y, 35.0f), Quaternion(), 4.0f, Vector2(5.0f, 15.0f));

	prefabGenerator->CreateSphere(world, Vector3(115, y + 2, 35.0f), 1.0f, 0.6f, true);

	prefabGenerator->CreateSlipperyFloor(world, Vector3(100.0f, y, 65.0f), Quaternion(), Vector2(2.0f, 15.0f));

	float coinY = y + 1.5f;
	//Coins
	prefabGenerator->CreateScoreBonus(world, Vector3(87.5f, coinY, -49.5f));
	prefabGenerator->CreateScoreBonus(world, Vector3(82.5f, coinY, -49.5f));

	prefabGenerator->CreateScoreBonus(world, Vector3(85.0f, coinY, -35.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(85.0f, coinY, -25.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(85.0f, coinY, -15.0f));

	prefabGenerator->CreateScoreBonus(world, Vector3(115.5f, y + 7.0f, -25.0f));

	prefabGenerator->CreateScoreBonus(world, Vector3(105.0f, coinY, 15.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(85.0f, coinY, 25.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(95.0f, coinY, 45.0f));
	prefabGenerator->CreateScoreBonus(world, Vector3(115.0f, coinY, 35.0f));
}

void Game::InitPracticePlayers() {
	player = prefabGenerator->CreatePlayer(this, Vector3(-100, 5, 100));
}

void Game::InitPracticeCheckpoints() {
	checkpoints.push_back(new Checkpoint(Vector3(-100, 10, -100), Vector3(20, 10, 20), 1));
	checkpoints.push_back(new Checkpoint(Vector3(100, 111, -100), Vector3(20, 10, 20), 2));

	goal = new Checkpoint(Vector3(100, 111, 100), Vector3(20, 10, 20), 2);
}
