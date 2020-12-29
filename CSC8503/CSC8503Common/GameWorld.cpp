#include "GameWorld.h"
#include "GameObject.h"
#include "Constraint.h"
#include "CollisionDetection.h"
#include "../../Common/Camera.h"
#include <algorithm>

using namespace NCL;
using namespace NCL::CSC8503;

GameWorld::GameWorld() {
	mainCamera = new Camera();
	objectTree = new QuadTree<GameObject*>(Vector2(1024, 1024), 7, 6);
	staticObjectTree = new QuadTree<GameObject*>(Vector2(1024, 1024), 7, 6);
	shuffleConstraints	= false;
	shuffleObjects		= false;
	worldIDCounter		= 0;
}

GameWorld::~GameWorld()	{
	delete objectTree;
	delete staticObjectTree;
}

void GameWorld::Clear() {
	gameObjects.clear();
	constraints.clear();
	killPlanes.clear();
	staticObjectTree->Clear();
	objectTree->Clear();
}

void GameWorld::ClearAndErase() {
	for (auto& i : gameObjects) {
		delete i;
	}
	for (auto& i : constraints) {
		delete i;
	}
	for (auto& i : killPlanes) {
		delete i;
	}

	Clear();
}

GameObject* GameWorld::AddGameObject(GameObject* o) {
	gameObjects.emplace_back(o);
	o->SetWorldID(worldIDCounter++);
	
	if (o->IsStatic()) {
		o->UpdateBroadphaseAABB();
		Vector3 halfSize;
		if (o->GetBroadphaseAABB(halfSize)) {
			staticObjectTree->Insert(o, o->GetTransform().GetPosition(), halfSize);
		}
	}

	return o;
}

void GameWorld::RemoveGameObject(GameObject* o, bool andDelete) {
	gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), o), gameObjects.end());
	if (andDelete) {
		delete o;
	}
}

void GameWorld::AddKillPlane(Plane* p) {
	killPlanes.emplace_back(p);
}

void GameWorld::RemoveKillPlane(Plane* p, bool andDelete) {
	killPlanes.erase(std::remove(killPlanes.begin(), killPlanes.end(), p), killPlanes.end());
	if (andDelete)
		delete p;
}

void GameWorld::GetObjectIterators(
	GameObjectIterator& first,
	GameObjectIterator& last) const {

	first	= gameObjects.begin();
	last	= gameObjects.end();
}

void GameWorld::OperateOnContents(GameObjectFunc f) {
	for (GameObject* g : gameObjects) {
		f(g);
	}
}

void GameWorld::UpdateWorld(float dt) {

	objectTree->Clear();
	
	for (auto g : gameObjects) {
		g->Update(dt);
		if (!g->IsStatic() && g->IsActive()) {
			g->UpdateBroadphaseAABB();

			for (auto p : killPlanes) {
				if (p->IsBehindPlane(g->GetTransform().GetPosition())) {
					std::cout << "Object \"" << g->GetName() << "\" is out of bounds.\n";
					g->OnKill();
				}
			}

			Vector3 halfSizes;
			if (!g->GetBroadphaseAABB(halfSizes)) {
				continue;
			}

			Vector3 pos = g->GetTransform().GetPosition();
			objectTree->Insert(g, pos, halfSizes);
		}	
	}
	objectTree->DebugDraw();


	if (shuffleObjects) {
		std::random_shuffle(gameObjects.begin(), gameObjects.end());
	}

	if (shuffleConstraints) {
		std::random_shuffle(constraints.begin(), constraints.end());
	}
}

bool GameWorld::Raycast(Ray& r, RayCollision& closestCollision, bool closestObject) const {
	RayCollision collision;

	std::set<GameObject*> possibleCollisions = objectTree->GetPossibleRayCollisions(r);

	for (auto& i : possibleCollisions) {
		if (!i->GetBoundingVolume() || i->GetCollisionLayer() & 1) { //objects might not be collideable etc...
			continue;
		}
		RayCollision thisCollision;
		if (CollisionDetection::RayIntersection(r, *i, thisCollision)) {

			if (!closestObject) {
				closestCollision = collision;
				closestCollision.node = i;
				return true;
			}
			else {
				if (thisCollision.rayDistance < collision.rayDistance) {
					thisCollision.node = i;
					collision = thisCollision;
				}
			}
		}
	}
	if (collision.node) {
		closestCollision = collision;
		closestCollision.node = collision.node;
		return true;
	}
	return false;
}


/*
Constraint Tutorial Stuff
*/

void GameWorld::AddConstraint(Constraint* c) {
	constraints.emplace_back(c);
}

void GameWorld::RemoveConstraint(Constraint* c, bool andDelete) {
	constraints.erase(std::remove(constraints.begin(), constraints.end(), c), constraints.end());
	if (andDelete) {
		delete c;
	}
}

void GameWorld::GetConstraintIterators(
	std::vector<Constraint*>::const_iterator& first,
	std::vector<Constraint*>::const_iterator& last) const {
	first	= constraints.begin();
	last	= constraints.end();
}