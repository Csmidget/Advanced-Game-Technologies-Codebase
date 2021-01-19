#include "KatamariAIBehaviourTree.h"

#include "BonusObject.h"
#include "AIObject.h"
#include "Game.h"

#include "../CSC8503Common/BehaviourAction.h"
#include "../CSC8503Common/BehaviourSequence.h"
#include "../CSC8503Common/BehaviourSelector.h"

using namespace NCL::CSC8503;

//An incredibly simple behaviour tree.
KatamariAIBehaviourTree::KatamariAIBehaviourTree(Game* g, AIObject* a) : BehaviourParallel("Race AI Root") {
	game = g;
	actor = a;

	BehaviourSequence* root = new BehaviourSequence("root");
	AddChild(root);

	root->AddChild(new BehaviourAction("Hunt nearest boid", [&](float dt, BehaviourState state)->BehaviourState {
		actor->SetCurrentState("Chasing Boids");
		auto boids = game->GetWorld()->ObjectsWithinRadius(actor->GetTransform().GetPosition(), 100.0f, "boid");
		if (boids.empty())
			return BehaviourState::Failure;

		for (auto boid : boids) {
			if (boid->GetTransform().GetPosition().LengthSquared() < 2025) { //45 squared 
				auto target = boid;
				actor->SetGoal(target->GetTransform().GetPosition());
				return BehaviourState::Ongoing;
			}
		}		
		return BehaviourState::Failure;
	}));
}