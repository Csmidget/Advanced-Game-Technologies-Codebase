#include "RaceAIBehaviourTree.h"
#include "../CSC8503Common/BehaviourAction.h"
#include "RaceBehaviourGetBonus.h"
#include "AIObject.h"

#include "../CSC8503Common/BehaviourSelector.h"

using namespace NCL::CSC8503;

RaceAIBehaviourTree::RaceAIBehaviourTree(Game* g, AIObject* a) : BehaviourParallel("Race AI Root") {
	game = g;
	actor = a;

	BehaviourSelector* root = new BehaviourSelector("root");

	root->AddChild(new RaceBehaviourGetBonus(game, actor));

	root->AddChild(new BehaviourAction("Go to goal", [&](float dt, BehaviourState state)->BehaviourState {
		actor->SetGoal(Vector3(-101.0f, 0.0f, -101.0f));
		return BehaviourState::Success;
	}));

	AddChild(root);

}