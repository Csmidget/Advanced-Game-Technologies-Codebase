#include "PrefabGenerator.h"
#include "../../Common/TextureLoader.cpp"


using namespace NCL;
using namespace CSC8503;

PrefabGenerator::PrefabGenerator() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("cube.msh", &cubeMesh);
	loadFunc("sphere.msh", &sphereMesh);
	loadFunc("Male1.msh", &charMeshA);
	loadFunc("courier.msh", &charMeshB);
	loadFunc("security.msh", &enemyMesh);
	loadFunc("coin.msh", &bonusMesh);
	loadFunc("capsule.msh", &capsuleMesh);

	basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");
}

PrefabGenerator::~PrefabGenerator() {
	delete cubeMesh;
	delete sphereMesh;
	delete charMeshA;
	delete charMeshB;
	delete enemyMesh;
	delete bonusMesh;

	delete basicTex;
	delete basicShader;
}


GameObject* PrefabGenerator::CreateAABBCube(Vector3 position) {
	return nullptr;
}

GameObject* PrefabGenerator::CreateOBBCube(Vector3 position, Quaternion orientation) {
	return nullptr;
}
GameObject* PrefabGenerator::CreateFloor(Vector3 position) {
	return nullptr;
}
GameObject* PrefabGenerator::CreateCapsule(Vector3 position, Quaternion orientation) {
	return nullptr;
}
GameObject* PrefabGenerator::CreateSphere(Vector3 position) {
	return nullptr;
}
GameObject* PrefabGenerator::CreateOrientedFloor(Vector3 position, Quaternion orientation) {
	return nullptr;
}
GameObject* PrefabGenerator::CreateTreadmill(Vector3 position) {
	return nullptr;
}
GameObject* PrefabGenerator::CreateBouncePad(Vector3 position, Quaternion orientation) {
	return nullptr;
}