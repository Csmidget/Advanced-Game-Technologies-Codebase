#pragma once

#include "GameTechRenderer.h"

namespace NCL {
	namespace CSC8503 {
		class PrefabGenerator {

		public:
			PrefabGenerator();
			~PrefabGenerator();


			GameObject* CreateAABBCube(Vector3 position, Vector3 dimensions, float inverseMass = 10.0f, bool isStatic = false);
			GameObject* CreateOBBCube(Vector3 position, Quaternion orientation, Vector3 dimensions, float inverseMass = 10.0f, bool respawning = false, bool isStatic = false);
			GameObject* CreateFloor(Vector3 position, Vector2 dimensions);
			GameObject* CreateOrientedFloor(Vector3 position, Quaternion orientation, Vector2 dimensions);
			GameObject* CreateCapsule(Vector3 position, Quaternion orientation, float halfHeight, float radius, float inverseMass = 10.0f, bool respawning = false, bool isStatic = false);
			GameObject* CreateSphere(Vector3 position, float radius, float inverseMass = 10.0f, bool respawning = false, bool isStatic = false);
			GameObject* CreateTreadmill(GameWorld* world, const Vector3& position, const Quaternion& orientation, float strength = 0.1f, const Vector2& dimensions = { 1,2 });
			GameObject* CreateBouncePad(GameWorld* world, const Vector3& position, const Quaternion& orientation, float strength = 10.0f, const Vector2& dimensions = { 1,1 });


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
			OGLMesh* bonusMesh = nullptr;
		};
	}
}




