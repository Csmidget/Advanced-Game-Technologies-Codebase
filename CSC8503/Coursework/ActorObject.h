#pragma once

#include "RespawningObject.h"
#include "../../Common/Vector2.h"
#include <map>

namespace NCL {

	namespace CSC8503 {

		class Constraint;
		class GameWorld;
		class Checkpoint;

		class ActorObject : public RespawningObject {

		public:
			ActorObject(GameWorld* world, Vector3 respawnPosition, string objectName) :RespawningObject(respawnPosition, objectName,false) {
				tags.push_back("actor");
			}

		    void Update(float dt) override;
			virtual void OnUpdate(float dt) = 0;
			virtual void Respawn();

			void AddScore(int scoreToAdd) { score += scoreToAdd; }
			int GetScore() const { return score; }

			void AddTimedConstraint(Constraint* constraint, float time);

			void SetCheckpoint(Checkpoint* cp) { currentCheckpoint = cp; }
			const Checkpoint* GetCheckpoint() const { return currentCheckpoint; }

		protected:
			int score;
			GameWorld* world;
			std::map<Constraint*, float> timedConstraints;

		private:
			Checkpoint* currentCheckpoint;
		};
	}
}