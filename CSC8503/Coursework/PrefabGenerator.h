#pragma once

#include "GameTechRenderer.h"

namespace NCL {
	namespace CSC8503 {
		class PrefabGenerator {

		public:
			PrefabGenerator();
			~PrefabGenerator();


			GameObject* CreateAABBCube(Vector3 position,Vector3 halfDimensions, bool isStatic);
			GameObject* CreateOBBCube(Vector3 position, Vector3 halfDimensions, Quaternion orientation, bool isStatic);
			GameObject* CreateFloor(Vector3 position, Vector2 scale, bool isStatic);
			GameObject* CreateOrientedFloor(Vector3 position,Vector2 scale, Quaternion orientation, bool isStatic);
			GameObject* CreateCapsule(Vector3 position, Quaternion orientation, bool isStatic);
			GameObject* CreateSphere(Vector3 position, float radius, bool isStatic);
			GameObject* CreateTreadmill(Vector3 position, Quaternion orientation);
			GameObject* CreateBouncePad(Vector3 position, Quaternion orientation);


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
		}

	}
}




