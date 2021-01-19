#include "Game.h"
#include "ActorObject.h"
#include "PlayerObject.h"

void Game::InitKatamariWorld() {
	Clear();
	InitKatamariBaseGeometry();
	InitKatamariKillPlanes();
	InitKatamariPlayers(1);
}

void Game::InitKatamariBaseGeometry() {
	prefabGenerator->CreateFloor(world, Vector3(0, -0.5f, 0), Vector2(50, 50));

	prefabGenerator->CreateAABBCube(world, Vector3(  0, 3,  48), Vector3(50, 3, 2), 0.0f, true);
	prefabGenerator->CreateAABBCube(world, Vector3(  0, 3, -48), Vector3(50, 3, 2), 0.0f, true);
	prefabGenerator->CreateAABBCube(world, Vector3( 48, 3,   0), Vector3(2, 3, 50), 0.0f, true);
	prefabGenerator->CreateAABBCube(world, Vector3(-48, 3,   0), Vector3(2, 3, 50), 0.0f, true);

	//Slightly higher gravity than normal to reduce chances of being knocked out of the arena.
	physics->SetGravity(Vector3(0, -20.0f, 0));
}
void Game::InitKatamariKillPlanes() {
	world->AddKillPlane(new Plane(Vector3(0, 1, 0), Vector3(0, -5, 0)));
}
void Game::InitKatamariPlayers(int opponentCount) {
	player = prefabGenerator->CreatePlayer(this, Vector3(45, 5, 45));
	player->SetSpeed(5.5f);

    opponents.push_back(prefabGenerator->CreateKatamariAI(this, Vector3(-45, 2.0f, -45), "Enemy 1"));
    opponents.push_back(prefabGenerator->CreateKatamariAI(this, Vector3(45, 2.0f, -45), "Enemy 2"));
    opponents.push_back(prefabGenerator->CreateKatamariAI(this, Vector3(-45, 2.0f, 45), "Enemy 3"));
}