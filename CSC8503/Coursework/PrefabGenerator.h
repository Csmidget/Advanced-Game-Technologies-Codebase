#pragma once

#include "GameTechRenderer.h"

namespace NCL {
	namespace CSC8503 {

		class PlayerObject;
		class AIObject;
		class Game;

		class PrefabGenerator {

		public:
			PrefabGenerator();
			~PrefabGenerator();



			GameObject*		CreateAABBCube(GameWorld* world, Vector3 position, Vector3 dimensions, float inverseMass = 10.0f, bool isStatic = false);
			GameObject*		CreateOBBCube(GameWorld* world, Vector3 position, Quaternion orientation, Vector3 dimensions, float inverseMass = 10.0f, bool respawning = false, bool isStatic = false);
			GameObject*		CreateSphere(GameWorld* world, Vector3 position, float radius, float inverseMass = 10.0f, bool respawning = false, bool isStatic = false);
			GameObject*		CreateCapsule(GameWorld* world, Vector3 position, Quaternion orientation, float halfHeight, float radius, float inverseMass = 10.0f, bool respawning = false, bool isStatic = false);

			GameObject*		CreateFloor(GameWorld* world, Vector3 position, Vector2 dimensions);
			GameObject*		CreateOrientedFloor(GameWorld* world, Vector3 position, Quaternion orientation, Vector2 dimensions,std::string name = "orientedFloor");
			GameObject*		CreateSlipperyFloor(GameWorld* world, const Vector3& position, const Quaternion& orientation, const Vector2& dimensions);
			GameObject*		CreateSwampFloor(GameWorld* world, const Vector3& position,const Quaternion& orientation,const Vector2& dimensions);

			GameObject*		CreateAnchor(GameWorld* world, const Vector3& position);
			GameObject*		CreatePathBlocker(GameWorld* world, const Vector2& position, const Vector2& halfDimensions);

			GameObject*		CreateSpinningBlock(GameWorld* world, const Vector3& position, const Vector3& upVector, float force, float length = 10.0f);
			void			CreatePendulum(GameWorld* world, Vector3 position, float distance, Vector3 force);

			GameObject*		CreateBouncePad(GameWorld* world, const Vector3& position, const Quaternion& orientation, float strength = 10.0f, const Vector2& dimensions = { 1,1 });
			GameObject*		CreateTreadmill(GameWorld* world, const Vector3& position, const Quaternion& orientation, float strength = 0.1f, const Vector2& dimensions = { 1,2 });

			GameObject*		CreateScoreBonus(GameWorld* world, Vector3 position, float respawnDelay = 5.0f);

			PlayerObject*	CreatePlayer(Game* game, const Vector3& position);
			AIObject*		CreateAI(Game* game, const Vector3& position,std::string name,float coinHuntRange = 30.0f, float angerThreshold = 3.0f, float strength = 2.0f);

		protected:

			OGLMesh* capsuleMesh = nullptr;
			OGLMesh* cubeMesh = nullptr;
			OGLMesh* sphereMesh = nullptr;
			OGLTexture* basicTex = nullptr;
			OGLShader* basicShader = nullptr;

			//Coursework Meshes
			OGLMesh* charMeshA = nullptr;
			OGLMesh* charMeshB = nullptr;
			OGLMesh* enemyMesh = nullptr;
			OGLMesh* coinMesh = nullptr;
			OGLMesh* bonusMesh = nullptr;
		};
	}
}




