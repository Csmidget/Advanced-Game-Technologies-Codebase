#include "Game.h"
#include "PrefabFactory.h"
#include "../CSC8503Common/NavigationMesh.h"
#include "Checkpoint.h"
#include "AIObject.h"


void Game::InitNavmeshWorld() {
	Clear();

	InitNavmeshLvlBaseGeometry();
	InitNavmeshLvlKillPlanes();
	InitNavmeshLvlPlayers(1);

	navMap = new NavigationMesh("simple.navmesh");

}

void Game::InitNavmeshLvlBaseGeometry() {

	prefabFactory->CreateFloor(world, Vector3(0, 0, 0), Vector2(40, 40));

	prefabFactory->CreateAABBCube(world, Vector3( 26, 2, -13), Vector3(14, 2, 27), 0.0f, true);
	prefabFactory->CreateAABBCube(world, Vector3(-10, 2, -5), Vector3(2, 2, 15), 0.0f, true);
	prefabFactory->CreateAABBCube(world, Vector3(0, 2, 12), Vector3(12, 2, 2), 0.0f, true);
	prefabFactory->CreateOBBCube(world, Vector3(26, 0.25, 17.35f),Matrix4::Rotation(25,Vector3(1,0,0)), Vector3(12, 2, 4.61f), 0.0f,false,true);

	prefabFactory->CreateScoreBonus(world, Vector3(-37, 1, 37), 10.0f);


	goal = new Checkpoint(Vector3(26.5, 4, -34.61), Vector3(5, 5, 5), 1);

}

void Game::InitNavmeshLvlKillPlanes() {

}

void Game::InitNavmeshLvlPlayers(int opponentCount) {
	opponents.push_back(prefabFactory->CreateRaceAI(this, Vector3(0, 2, 0), "Enemy 1",40.0f,40.0f));
	opponents[0]->SetSpeed(3.0f);
}