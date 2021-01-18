#include "BoidObject.h"
#include "ActorObject.h"
#include "BoidSwarm.h"
#include "Game.h"

using namespace NCL::CSC8503;

BoidObject::BoidObject(Game* game, BoidSwarm** swarm, Vector3 respawnPosition) : RespawningObject(respawnPosition,"boid") {
	this->swarm = swarm;
	this->game = game;

	tags.push_back("boid");

	if (*swarm)
		(*swarm)->AddBoid(this);
}

BoidObject::~BoidObject() {
	if ((*swarm))
		(*swarm)->RemoveBoid(this);
}

void BoidObject::Update(float dt) {

	if (!(*swarm))
		return;

	Vector3 sumofPoints = (*swarm)->SumOfPoints();

	//Remove this objects contribution to the sum
	sumofPoints -= transform.GetPosition();

	//Find the average position of all other boids in the swarm.
	Vector3 targetPos = sumofPoints / ((*swarm)->BoidCount() - 1);

	//Move towards centre of swarm
	Vector3 targetDir = (targetPos - transform.GetPosition()) / 100.0f;


	Vector3 targetVelocity = (*swarm)->SumOfVelocities();
	targetVelocity -= physicsObject->GetLinearVelocity();
	targetVelocity /= ((*swarm)->BoidCount() - 1);
	targetVelocity = (targetVelocity - physicsObject->GetLinearVelocity()) / 8;

	Vector3 boidAvoidDir;
	auto nearbyBoids = game->GetWorld()->ObjectsWithinRadius(transform.GetPosition(), 5.0f, "boid");
	for (auto boid : nearbyBoids)
	{
		if (boid == this)
			continue;

		Vector3 displacement = boid->GetTransform().GetPosition() - transform.GetPosition();
		float strength = 1 / exp(displacement.Length());

		boidAvoidDir -= displacement * strength;
	}

	for (auto target : (*swarm)->avoidTargets) {
		Vector3 displacement = target->GetTransform().GetPosition() - transform.GetPosition();

		if (displacement.Length() < 10.0f) {
			boidAvoidDir -= displacement * 2;
		}
	}


	Vector3 finalDir = (targetDir + boidAvoidDir + targetVelocity);
	finalDir.y = 0.0f;
	finalDir.Normalise();

	physicsObject->AddForce(finalDir * 0.8f);
}


void BoidObject::OnCollisionBegin(GameObject* otherObject) {
	
	if (otherObject->HasTag("actor")) {
		auto actor = (ActorObject*)otherObject;

		actor->AddScore(1);
		actor->AddSpeed(0.1f);
		actor->GetTransform().SetScale(actor->GetTransform().GetScale() + Vector3(0.1, 0.1, 0.1));

		CapsuleVolume* volume = (CapsuleVolume*)actor->GetBoundingVolume();
		volume->SetHalfHeight(volume->GetHalfHeight() + 0.08f);
		volume->SetRadius(volume->GetRadius() + 0.08f);
	
		isActive = false;

		std::cout << "boid caught";
	}

}