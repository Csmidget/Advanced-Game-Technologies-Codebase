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

	float speed = 0.8f;

	if (!(*swarm))
		return;

	Vector3 sumofPoints = (*swarm)->SumOfPoints();
	Vector3 currPos = transform.GetPosition();

	//Move towards centre of swarm
	Vector3 swarmCentreDir;
	{
		float swarmCentreStrength = 0.005f;

		sumofPoints -= currPos;

		//Find the average position of all other boids in the swarm.
		Vector3 targetPos = sumofPoints / ((*swarm)->BoidCount() - 1);

		swarmCentreDir = (targetPos - currPos) * swarmCentreStrength;
	}

	//Attempt to match velocity with other boids
	Vector3 matchVelocity = (*swarm)->SumOfVelocities();
	{
		float matchVelocityStrength = 0.05f;

		matchVelocity -= physicsObject->GetLinearVelocity();
		matchVelocity /= ((*swarm)->BoidCount() - 1);
		matchVelocity = (matchVelocity - physicsObject->GetLinearVelocity()) * matchVelocityStrength;
	}

	//Avoid other nearby boids as well as targets swarm has been assigned to avoid.
	Vector3 avoidDir;
	{
		float avoidBoidDistance = 20.0f;
		float avoidTargetsDistance = 12.0f;
		float avoidStrength = 2.5f;
		auto nearbyBoids = game->GetWorld()->ObjectsWithinRadius(currPos, avoidBoidDistance, "boid");

		for (auto boid : nearbyBoids)
		{
			if (boid == this)
				continue;

			Vector3 displacement = boid->GetTransform().GetPosition() - currPos;

			//Falls of exponentially with distance.
			float strength = 1 / exp(displacement.Length());
			avoidDir -= displacement * strength;
		}

		for (auto target : (*swarm)->avoidTargets) {
			Vector3 displacement = target->GetTransform().GetPosition() - currPos;

			if (displacement.Length() < avoidTargetsDistance) {
				avoidDir -= displacement * avoidStrength;
			}
		}
	}

	//Tend slightly towards the center. This is to avoid boids getting clumped in edges and corners.
	float worldCentreStrength = 0.003f;
	Vector3 centreDir = (Vector3() - currPos) * worldCentreStrength;

	//Combine our calculated forces and normalise to get a single direction vector
	Vector3 finalDir = swarmCentreDir + avoidDir + matchVelocity + centreDir;
	finalDir.y = 0.0f;
	finalDir.Normalise();

	physicsObject->AddForce(finalDir * speed);
}

void BoidObject::OnCollisionBegin(GameObject* otherObject) {
	
	if (otherObject->HasTag("actor")) {
		auto actor = (ActorObject*)otherObject;

		actor->AddScore(1);
		actor->AddSpeed(0.02f);
		actor->GetTransform().SetScale(actor->GetTransform().GetScale() + Vector3(0.1, 0.1, 0.1));

		CapsuleVolume* volume = (CapsuleVolume*)actor->GetBoundingVolume();
		volume->SetHalfHeight(volume->GetHalfHeight() + 0.08f);
		volume->SetRadius(volume->GetRadius() + 0.03f);
	
		isActive = false;
	}
}