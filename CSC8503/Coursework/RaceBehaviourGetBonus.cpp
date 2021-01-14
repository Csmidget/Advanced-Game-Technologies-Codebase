#include "RaceBehaviourGetBonus.h"
#include "BonusObject.h"
#include "AIObject.h"
#include "Game.h"
#include "../CSC8503Common/BehaviourAction.h"
using namespace NCL::CSC8503;

RaceBehaviourGetBonus::RaceBehaviourGetBonus(Game* g, AIObject* a) :BehaviourSequence("Get Bonus") {
	game = g;
	actor = a;
	this->bonusTarget = nullptr;

	BehaviourAction* locateBonus = new BehaviourAction("Locate Bonus",[&](float dt, BehaviourState state)->BehaviourState {

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
	AddChild(locateBonus);

	BehaviourAction* moveToBonus = new BehaviourAction("Move To Bonus", [&](float dt, BehaviourState state)->BehaviourState {

		Vector3 targetPosition = bonusTarget->GetTransform().GetPosition();
		targetPosition.y = 0.0f;

		if (!actor->SetGoal(targetPosition,actor->GetCoinHuntRange())) {
			bonusTarget = nullptr;
			return BehaviourState::Failure;
		}

		if ((targetPosition - actor->GetTransform().GetPosition()).Length() < 1.0f) {
			return BehaviourState::Success;
		}
		else {
			return BehaviourState::Ongoing;
		}

	});
	AddChild(moveToBonus);
}
