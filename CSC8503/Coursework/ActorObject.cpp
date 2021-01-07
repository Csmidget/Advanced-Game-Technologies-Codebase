#include "ActorObject.h" 
#include "Checkpoint.h"
#include "../CSC8503Common/GameWorld.h"

using namespace NCL::CSC8503;

void ActorObject::Update(float dt) {
	for (auto timedConstraint : timedConstraints) {

		timedConstraint.second -= dt;

		if (timedConstraint.second <= 0.0f) {
			timedConstraints.erase(timedConstraint.first);
			world->RemoveConstraint(timedConstraint.first, true);
		}
	}

	OnUpdate(dt);
}

void ActorObject::AddTimedConstraint(Constraint* constraint, float time) {
	timedConstraints.emplace(constraint, time);
}

void ActorObject::Respawn() {

	//If we don't have a checkpoint assigned, fallback to default respawningObject behaviour
	if (!currentCheckpoint) {
		RespawningObject::Respawn();
		return;
	}

	//Select a random spawn point from the available spawnpoints at current checkpoint.
	SetRespawnPosition(currentCheckpoint->GetSpawnPoint());

	//Now respawn at that point.
	RespawningObject::Respawn();
}