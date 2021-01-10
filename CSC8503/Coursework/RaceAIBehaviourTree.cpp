#include "RaceAIBehaviourTree.h"
#include "RaceBehaviourGetBonus.h"

#include "../CSC8503Common/BehaviourSelector.h"

using namespace NCL::CSC8503;

RaceAIBehaviourTree::RaceAIBehaviourTree(Game* game, GameObject* actor) : BehaviourParallel("Race AI Root") {

	BehaviourSelector* root = new BehaviourSelector("root");

	root->AddChild(new RaceBehaviourGetBonus(game, actor));

	AddChild(root);

}