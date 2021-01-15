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


			//These functions create a new object, without adding them to a world.
			GameObject* CreateAABBCube(Vector3 position, Vector3 dimensions, float inverseMass = 10.0f, bool isStatic = false);
			GameObject* CreateOBBCube(Vector3 position, Quaternion orientation, Vector3 dimensions, float inverseMass = 10.0f, bool respawning = false, bool isStatic = false);
			GameObject* CreateFloor(Vector3 position, Vector2 dimensions);
			GameObject* CreateOrientedFloor(Vector3 position, Quaternion orientation, Vector2 dimensions,std::string name = "orientedFloor");
			GameObject* CreateCapsule(Vector3 position, Quaternion orientation, float halfHeight, float radius, float inverseMass = 10.0f, bool respawning = false, bool isStatic = false);
			GameObject* CreateSphere(Vector3 position, float radius, float inverseMass = 10.0f, bool respawning = false, bool isStatic = false);
			GameObject* CreateAnchor(const Vector3& position);
			GameObject* CreateSlipperyFloor(const Vector3& position, const Quaternion& orientation, const Vector2& dimensions);
			GameObject* CreateSwampFloor(const Vector3& position,const Quaternion& orientation,const Vector2& dimensions);

			//These functions add objects directly to a world (multiple component objects, or objects including constraints).
			GameObject*		AddBouncePad(GameWorld* world, const Vector3& position, const Quaternion& orientation, float strength = 10.0f, const Vector2& dimensions = { 1,1 });
			GameObject*		AddTreadmill(GameWorld* world, const Vector3& position, const Quaternion& orientation, float strength = 0.1f, const Vector2& dimensions = { 1,2 });
			GameObject*		AddSpinningBlock(GameWorld* world, const Vector3& position, const Vector3& upVector, float force);
			GameObject*		AddScoreBonus(GameWorld* world, Vector3 position, float respawnDelay = 5.0f);
			PlayerObject*	AddPlayer(Game* game, const Vector3& position);
			AIObject*		AddAI(Game* game, const Vector3& position,float coinHuntRange = 30.0f, float angerThreshold = 3.0f, float strength = 2.0f);
			void			AddPendulum(GameWorld* world, Vector3 position, float distance, Vector3 force);

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




