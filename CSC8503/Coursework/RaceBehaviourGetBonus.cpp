#include "RaceBehaviourGetBonus.h"
#include "BonusObject.h"
#include "Game.h"
#include "../CSC8503Common/BehaviourAction.h"
using namespace NCL::CSC8503;

RaceBehaviourGetBonus::RaceBehaviourGetBonus(Game* g, GameObject* a) :BehaviourSequence("Get Bonus") {
	game = g;
	actor = a;
	this->bonusTarget = nullptr;

	BehaviourAction* locateBonus = new BehaviourAction("Locate Bonus",[&](float dt, BehaviourState state)->BehaviourState {

		//If we already have a target, and they are still available, keep going
		if (bonusTarget && !bonusTarget->IsPopped()) {
			return BehaviourState::Success;
		}

		//Find a target within range
		bonusTarget = (BonusObject*)game->GetWorld()->ClosestObjectWithinRadius(actor->GetTransform().GetPosition(), 10.0f, "bonus");

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
		Vector3 direction = bonusTarget->GetTransform().GetPosition() - actor->GetTransform().GetPosition();
		direction.Normalise();

		actor->GetPhysicsObject()->AddForce(direction * 1.0f);

		if ((bonusTarget->GetTransform().GetPosition() - actor->GetTransform().GetPosition()).Length() < 1.0f)
			return BehaviourState::Success;
		else
			return BehaviourState::Failure;
	});
	AddChild(moveToBonus);
}
