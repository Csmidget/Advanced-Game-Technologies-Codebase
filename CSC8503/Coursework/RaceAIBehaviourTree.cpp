#include "RaceAIBehaviourTree.h"
#include "../CSC8503Common/BehaviourAction.h"
#include "../CSC8503Common/BehaviourSequence.h"
#include "BonusObject.h"
#include "AIObject.h"
#include "Game.h"

#include "../CSC8503Common/BehaviourSelector.h"

using namespace NCL::CSC8503;

RaceAIBehaviourTree::RaceAIBehaviourTree(Game* g, AIObject* a) : BehaviourParallel("Race AI Root") {
	game = g;
	actor = a;

	BehaviourSelector* root = new BehaviourSelector("root");
	AddChild(root);

	root->AddChild(CreateAngerSequence());

	root->AddChild(CreateBonusSequence());

	root->AddChild(new BehaviourAction("Go to goal", [&](float dt, BehaviourState state)->BehaviourState {
		actor->SetCurrentState("Seeking goal");
		actor->SetGoal(Vector3(-101.0f, 0.0f, -101.0f));
		return BehaviourState::Success;
	}));
}

BehaviourSequence* RaceAIBehaviourTree::CreateBonusSequence() {

	BehaviourSequence* sequence = new BehaviourSequence("Bonus Sequence");

	BehaviourAction* locateBonus = new BehaviourAction("Locate Bonus", [&](float dt, BehaviourState state)->BehaviourState {

		//If we already have a target, and they are still available, keep going
		if (bonusTarget && !bonusTarget->IsPopped()) {
			return BehaviourState::Success;
		}

		auto bonusesInRange = game->GetWorld()->ObjectsWithinRadius(actor->GetTransform().GetPosition(), 10.0f, "bonus");

		if (bonusesInRange.size() == 0)
			return BehaviourState::Failure;

		for (GameObject* o : bonusesInRange)
		{
			if (!((BonusObject*)o)->IsPopped()) {
				bonusTarget = (BonusObject*)o;
			}
		}

		//If the target is available, go for it.
		if (bonusTarget && !bonusTarget->IsPopped()) {
			return BehaviourState::Success;
		}
		else {
			bonusTarget = nullptr;
			return BehaviourState::Failure;
		}
		});
	sequence->AddChild(locateBonus);

	BehaviourAction* moveToBonus = new BehaviourAction("Move To Bonus", [&](float dt, BehaviourState state)->BehaviourState {

		Vector3 targetPosition = bonusTarget->GetTransform().GetPosition();
		targetPosition.y = 0.0f;

		if (!actor->SetGoal(targetPosition, actor->GetCoinHuntRange())) {
			bonusTarget = nullptr;
			return BehaviourState::Failure;
		}

		if ((targetPosition - actor->GetTransform().GetPosition()).Length() < 1.0f) {
			return BehaviourState::Success;
		}
		else {
			actor->SetCurrentState("Seeking Coin");
			return BehaviourState::Ongoing;
		}

		});
	sequence->AddChild(moveToBonus);

	return sequence;
}

BehaviourSequence* RaceAIBehaviourTree::CreateAngerSequence() {
	BehaviourSequence* sequence = new BehaviourSequence("Anger Sequence");

	BehaviourAction* checkAnger = new BehaviourAction("Check Anger", [&](float dt, BehaviourState state)->BehaviourState {
		return (actor->IsAngry() ? BehaviourState::Success : BehaviourState::Failure);
	});
	sequence->AddChild(checkAnger);

	BehaviourAction* findTargetForAnger = new BehaviourAction("Find Anger Target", [&](float dt, BehaviourState state)->BehaviourState {

		auto actorsInRange = game->GetWorld()->ObjectsWithinRadius(actor->GetTransform().GetPosition(), 15.0f, "actor");

		if (actorsInRange.empty()) {
			return BehaviourState::Failure;
		}

		for (GameObject* o : actorsInRange)
		{
			//We are also an actor. So ignore ourselves.
			if (actor == o)
				continue;

			//Only go for a target it the path to them isn't too long.
			if (actor->SetGoal(o->GetTransform().GetPosition(),17.5f)) {
				return BehaviourState::Success;
			}
		}

		return BehaviourState::Failure;
	});
	sequence->AddChild(findTargetForAnger);

	return sequence;
}