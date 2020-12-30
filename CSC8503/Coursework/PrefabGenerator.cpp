#include "PrefabGenerator.h"
#include "ForceObject.h"
#include "RespawningObject.h"
#include "PlayerObject.h"
#include "../CSC8503Common/PositionConstraint.h"
#include "../CSC8503Common/OrientationConstraint.h"
#include "../CSC8503Common/AngularImpulseConstraint.h"

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

GameObject* PrefabGenerator::CreateOBBCube(Vector3 position, Quaternion orientation, Vector3 dimensions, float inverseMass,bool respawning, bool isStatic) {
	GameObject* cube = respawning ? new RespawningObject(position,true,"respawningOrientedCube") : new GameObject("orientedCube");

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
	cube->GetPhysicsObject()->SetElasticity(1.5f);
	cube->GetPhysicsObject()->SetFriction(0.7f);


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

GameObject* PrefabGenerator::CreateSlipperyFloor(const Vector3& position, const Quaternion& orientation,const Vector2& dimensions) {
	GameObject* floor = CreateOrientedFloor(position, orientation, dimensions);
	PhysicsObject* phys = floor->GetPhysicsObject();

	phys->SetFriction(0.0f);
	
	floor->GetRenderObject()->SetColour(Vector4(5, 5, 5, 1));

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
	capsule->GetPhysicsObject()->SetFriction(0.1f);

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
	sphere->GetPhysicsObject()->SetFriction(0.1f);


	sphere->SetIsStatic(isStatic);

	return sphere;
}

GameObject* PrefabGenerator::CreateAnchor(const Vector3& position) {
	GameObject* anchor = new GameObject("anchor");

	anchor->GetTransform().SetPosition(position);

	anchor->SetPhysicsObject(new PhysicsObject(&anchor->GetTransform(), nullptr));
	anchor->GetPhysicsObject()->SetInverseMass(0.0f);
	anchor->SetIsStatic(true);

	return anchor;
}


GameObject* PrefabGenerator::AddTreadmill(GameWorld* world, const Vector3& position, const Quaternion& orientation, float strength, const Vector2& dimensions) {
	return world->AddGameObject(new ForceObject(world, cubeMesh, basicTex, basicShader, position, Vector3(dimensions.x,0.1,dimensions.y),orientation, Vector3(0, 0, -1), strength));
}

GameObject* PrefabGenerator::AddBouncePad(GameWorld* world, const Vector3& position, const Quaternion& orientation, float strength, const Vector2& dimensions) {
	return world->AddGameObject(new ForceObject(world, cubeMesh, basicTex, basicShader, position, Vector3(dimensions.x, 0.1, dimensions.y), orientation, Vector3(0, 1, 0), strength));
}

GameObject* PrefabGenerator::AddSpinningBlock(GameWorld* world, const Vector3& position, const Vector3& upVector, float force) {

	GameObject* spinningBlock = world->AddGameObject(CreateOBBCube(position, Quaternion(), Vector3(12.5, 2, 2), 0.01f));

	GameObject* positionAnchor = world->AddGameObject(CreateAnchor(position));
	world->AddConstraint(new PositionConstraint(positionAnchor, spinningBlock, 0.0f));

	GameObject* orientationAnchor = world->AddGameObject(CreateAnchor(position + upVector));
	world->AddConstraint(new OrientationConstraint(spinningBlock, orientationAnchor, Vector3(0, 1, 0)));

	world->AddConstraint(new AngularImpulseConstraint(spinningBlock, Vector3(0,force,0)));

	return spinningBlock;
}

PlayerObject* PrefabGenerator::AddPlayer(GameWorld* world, const Vector3& position) {
	float meshSize = 1.0f;
	float inverseMass = 0.5f;

	PlayerObject* player = new PlayerObject(position,world->GetMainCamera());

	CapsuleVolume* volume = new CapsuleVolume(meshSize,0.3f);
	
	player->SetBoundingVolume((CollisionVolume*)volume);
	
	player->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);
	
	if (rand() % 2) {
		player->SetRenderObject(new RenderObject(&player->GetTransform(), charMeshA, nullptr, basicShader));
	}
	else {
		player->SetRenderObject(new RenderObject(&player->GetTransform(), charMeshB, nullptr, basicShader));
	}
	player->SetPhysicsObject(new PhysicsObject(&player->GetTransform(), player->GetBoundingVolume()));
	
	player->GetPhysicsObject()->SetInverseMass(5.0f);
	player->GetPhysicsObject()->InitCubeInertia();
	player->GetPhysicsObject()->SetElasticity(1.0f);
	player->GetPhysicsObject()->SetFriction(0.5f);


	
	world->AddGameObject(player);
		
	return player;
}