#pragma once

#include "RespawningObject.h"
#include "../../Common/Vector2.h"
#include <map>

namespace NCL {

	namespace CSC8503 {

		class Constraint;
		class Game;
		class Checkpoint;

		class ActorObject : public RespawningObject {

		public:
			ActorObject(Game* game, Vector3 respawnPosition, string objectName) :RespawningObject(respawnPosition, objectName,false) {
				this->game = game;
				tags.push_back("actor");
				lastCollisionTimer = 0.0f;
				currentCheckpoint = nullptr;
				score = 0;
			}

		    void Update(float dt) override;
			virtual void OnUpdate(float dt) = 0;
			virtual void Respawn();
			virtual void OnCollisionBegin(GameObject* otherObject) override;


			void AddScore(int scoreToAdd) { score += scoreToAdd; }
			int GetScore() const { return score; }

			void AddSpeed(float val) { speed += val; }
			void SetSpeed(float val) { speed = val; }
			float GetSpeed() const { return speed; }

			void AddTimedConstraint(Constraint* constraint, float time);

			void SetCheckpoint(Checkpoint* cp) { currentCheckpoint = cp; }
			const Checkpoint* GetCheckpoint() const { return currentCheckpoint; }

		protected:
			int score;
			Game* game;
			std::map<Constraint*, float> timedConstraints;
			float lastCollisionTimer;
			float speed;

		private:
			Checkpoint* currentCheckpoint;

		};
	}
}