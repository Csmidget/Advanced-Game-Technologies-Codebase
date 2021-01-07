#include "ScoreBonusObject.h"
#include "ActorObject.h"

using namespace NCL::CSC8503;

ScoreBonusObject::ScoreBonusObject(Vector3 respawnPosition, int score) : BonusObject(respawnPosition) {	
	this->score = score;
}

void ScoreBonusObject::ApplyBonus(ActorObject* otherObject) {
	otherObject->AddScore(score);
}