#include "PrefabGenerator.h"
#include "ForceObject.h"
#include "RespawningObject.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/TextureLoader.h"


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


GameObject* PrefabGenerator::CreateAABBCube(Vector3 position, Vector3 dimensions, float inverseMass, bool isStatic) {
	GameObject* cube = new GameObject("cube");

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	cube->SetIsStatic(isStatic);

	return cube;
}

GameObject* PrefabGenerator::CreateOBBCube(Vector3 position, Quaternion orientation, Vector3 dimensions, float inverseMass, bool isStatic) {
	GameObject* cube = new GameObject("orientedCube");

	OBBVolume* volume = new OBBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2)
		.SetOrientation(orientation);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	cube->SetIsStatic(isStatic);

	return cube;
}
GameObject* PrefabGenerator::CreateFloor(Vector3 position, Vector2 dimensions) {
	GameObject* floor = new GameObject("floor");

	Vector3 floorSize = Vector3(dimensions.x, 0.5, dimensions.y);
	AABBVolume* volume = new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	floor->SetIsStatic(true);


	return floor;
}
GameObject* PrefabGenerator::CreateOrientedFloor(Vector3 position, Quaternion orientation, Vector2 dimensions) {
	GameObject* floor = new GameObject("orientedFloor");

	Vector3 floorSize = Vector3(dimensions.x, 0.5f, dimensions.y);
	OBBVolume* volume = new OBBVolume(Vector3(floorSize));

	floor->SetBoundingVolume((CollisionVolume*)volume);

	floor->GetTransform()
		.SetPosition(position)
		.SetScale(floorSize * 2)
		.SetOrientation(orientation);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();
	floor->GetPhysicsObject()->SetFriction(0.1f);

	floor->SetIsStatic(true);

	return floor;
}
GameObject* PrefabGenerator::CreateCapsule(Vector3 position, Quaternion orientation, float halfHeight, float radius, float inverseMass, bool respawning, bool isStatic) {
	GameObject* capsule = respawning ? new RespawningObject(position,true,"respawningCapsule") : new GameObject("capsule");

	CapsuleVolume* volume = new CapsuleVolume(halfHeight, radius);
	capsule->SetBoundingVolume((CollisionVolume*)volume);

	capsule->GetTransform()
		.SetScale(Vector3(radius * 2, halfHeight, radius * 2))
		.SetPosition(position);

	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, basicTex, basicShader));
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));

	capsule->GetPhysicsObject()->SetInverseMass(inverseMass);
	capsule->GetPhysicsObject()->InitCubeInertia();
	capsule->GetPhysicsObject()->SetElasticity(1.0f);

	capsule->SetIsStatic(isStatic);

	return capsule;
}

GameObject* PrefabGenerator::CreateSphere(Vector3 position, float radius, float inverseMass, bool respawning, bool isStatic) {
	GameObject* sphere = respawning ? new RespawningObject(position,true,"respawningSphere") : new GameObject("sphere");

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitHollowSphereInertia();
	sphere->GetPhysicsObject()->SetElasticity(1.0f);


	sphere->SetIsStatic(isStatic);

	return sphere;
}
GameObject* PrefabGenerator::CreateTreadmill(GameWorld* world, const Vector3& position, const Quaternion& orientation, float strength, const Vector2& dimensions) {
	return new ForceObject(world, cubeMesh, basicTex, basicShader, position, Vector3(dimensions.x,0.1,dimensions.y),orientation, Vector3(0, 0, -1), strength);
}

GameObject* PrefabGenerator::CreateBouncePad(GameWorld* world, const Vector3& position, const Quaternion& orientation, float strength, const Vector2& dimensions) {
	return new ForceObject(world, cubeMesh, basicTex, basicShader, position, Vector3(dimensions.x, 0.1, dimensions.y), orientation, Vector3(0, 1, 0), strength);

}