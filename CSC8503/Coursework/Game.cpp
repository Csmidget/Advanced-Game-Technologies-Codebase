#include "Game.h"
#include "ForceObject.h"

#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/PositionConstraint.h"
#include "../CSC8503Common/OrientationConstraint.h"
#include "../CSC8503Common/AngularImpulseConstraint.h"


using namespace NCL;
using namespace CSC8503;

Game::Game() {
	world		= new GameWorld();
	renderer	= new GameTechRenderer(*world);
	physics		= new PhysicsSystem(*world);
	prefabGenerator = new PrefabGenerator();

	forceMagnitude	= 10.0f;
	useGravity		= false;
	inSelectionMode = false;

	Debug::SetRenderer(renderer);

	InitialiseAssets();
}

/*

Each of the little demo scenarios used in the game uses the same 2 meshes, 
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/
void Game::InitialiseAssets() {

	InitCamera();
	InitWorld();
}

Game::~Game()	{
	delete physics;
	delete renderer;
	delete world;
	delete prefabGenerator;
}

void Game::UpdateGame(float dt) {
	if (!inSelectionMode) {
		world->GetMainCamera()->UpdateCamera(dt);
	}

	UpdateKeys();

	if (useGravity) {
		Debug::Print("(G)ravity on", Vector2(5, 95));
	}
	else {
		Debug::Print("(G)ravity off", Vector2(5, 95));
	}

	SelectObject();
	MoveSelectedObject();
	physics->Update(dt);

	if (lockedObject != nullptr) {
		Vector3 objPos = lockedObject->GetTransform().GetPosition();
		Vector3 camPos = objPos + lockedOffset;

		Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0,1,0));

		Matrix4 modelMat = temp.Inverse();

		Quaternion q(modelMat);
		Vector3 angles = q.ToEuler(); //nearly there now!

		world->GetMainCamera()->SetPosition(camPos);
		world->GetMainCamera()->SetPitch(angles.x);
		world->GetMainCamera()->SetYaw(angles.y);

		//Debug::DrawAxisLines(lockedObject->GetTransform().GetMatrix(), 2.0f);
	}

	world->UpdateWorld(dt);
	renderer->Update(dt);

	Debug::FlushRenderables(dt);
	renderer->Render();
}

void Game::UpdateKeys() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F1)) {
		InitWorld(); //We can reset the simulation at any time with F1
		selectionObject = nullptr;
		lockedObject	= nullptr;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2)) {
		InitCamera(); //F2 will reset the camera to a specific default place
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G)) {
		useGravity = !useGravity; //Toggle gravity!
		physics->UseGravity(useGravity);
	}
	//Running certain physics updates in a consistent order might cause some
	//bias in the calculations - the same objects might keep 'winning' the constraint
	//allowing the other one to stretch too much etc. Shuffling the order so that it
	//is random every frame can help reduce such bias.
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9)) {
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10)) {
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F7)) {
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8)) {
		world->ShuffleObjects(false);
	}

	if (lockedObject) {
		LockedObjectMovement();
	}
	else {
		DebugObjectMovement();
	}
}

void Game::LockedObjectMovement() {
	Matrix4 view		= world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld	= view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();

	Vector3 charForward  = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);
	Vector3 charForward2 = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);

	float force = 100.0f;

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
		lockedObject->GetPhysicsObject()->AddForce(-rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
		Vector3 worldPos = selectionObject->GetTransform().GetPosition();
		lockedObject->GetPhysicsObject()->AddForce(rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
		lockedObject->GetPhysicsObject()->AddForce(fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
		lockedObject->GetPhysicsObject()->AddForce(-fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NEXT)) {
		lockedObject->GetPhysicsObject()->AddForce(Vector3(0,-10,0));
	}
}

void Game::DebugObjectMovement() {
//If we've selected an object, we can manipulate it with some key presses
	if (inSelectionMode && selectionObject) {
		//Twist the selected object!
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(-10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM7)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM8)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, -10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, -10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM5)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
		}
	}

}

void Game::InitCamera() {
	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(500.0f);
	world->GetMainCamera()->SetPitch(-15.0f);
	world->GetMainCamera()->SetYaw(315.0f);
	world->GetMainCamera()->SetPosition(Vector3(-60, 40, 60));
	lockedObject = nullptr;
}

void Game::Clear() {
	world->ClearAndErase();
	physics->Clear();
}

void Game::InitWorld() {
	Clear();

	InitKillPlanes();
	InitBaseGeometry();
	InitGauntlet1();
	InitSlope();
	InitGauntlet2();
}

void Game::InitKillPlanes() {
	//Kill plane below the floor
	world->AddKillPlane(new Plane(Vector3(0, 1, 0), Vector3(0, -5, 0)));
}

void Game::InitBaseGeometry() {

	//Starting zone
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(-100, -0.5f, 100), Vector2(25, 25)));

	//First Gauntlet
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(-100, -0.5f, 0.0f), Vector2(12.5, 75)));

	//Checkpoint1
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(-100, -0.5f, -100.0f), Vector2(25, 25)));

	//Slope
	world->AddGameObject(prefabGenerator->CreateOrientedFloor(Vector3(0.0f, 43.0f, -100.0f), Quaternion::EulerAnglesToQuaternion(0, 0, 30), Vector2(87, 12.5)));

	//Checkpoint2
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(100.0f, 86.35f, -100.0f), Vector2(25, 25)));

	//Second Gauntlet
	//world->AddGameObject(prefabGenerator->CreateFloor(Vector3(100.0f, 86.35f, 0.0f), Vector2(12.5, 75)));

	//Goal
	world->AddGameObject(prefabGenerator->CreateFloor(Vector3(100.0f, 86.35f, 100.0f), Vector2(25, 25)));

}

void Game::InitGauntlet1() {

	prefabGenerator->AddSpinningBlock(world, Vector3(-100, 3.0f, -30.0f), Vector3(0, 1, 0), -100.0f);
	prefabGenerator->AddSpinningBlock(world, Vector3(-100, 3.0f, 0.0f), Vector3(0, 1, 0), 100.0f);
	prefabGenerator->AddSpinningBlock(world, Vector3(-100, 3.0f, 30.0f), Vector3(0, 1, 0), -100.0f);
}

void Game::InitSlope() {

	for (int i = 1; i <= 4; ++i) { 
		float height = i * 150;

		world->AddGameObject(prefabGenerator->CreateCapsule(Vector3( 70.0f, height, -110.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true));
		world->AddGameObject(prefabGenerator->CreateCapsule(Vector3( 70.0f, height, -105.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true));
		world->AddGameObject(prefabGenerator->CreateCapsule(Vector3( 70.0f, height, -100.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true));
		world->AddGameObject(prefabGenerator->CreateCapsule(Vector3( 70.0f, height, - 95.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true));
		world->AddGameObject(prefabGenerator->CreateCapsule(Vector3( 70.0f, height, - 90.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true));

		world->AddGameObject(prefabGenerator->CreateSphere(Vector3(70.0f, height + 50, -110.0f), 1.0f, 0.2f, true));
		world->AddGameObject(prefabGenerator->CreateSphere(Vector3(70.0f, height + 50, -105.0f), 1.0f, 0.2f, true));
		world->AddGameObject(prefabGenerator->CreateSphere(Vector3(70.0f, height + 50, -100.0f), 1.0f, 0.2f, true));
		world->AddGameObject(prefabGenerator->CreateSphere(Vector3(70.0f, height + 50, - 95.0f), 1.0f, 0.2f, true));
		world->AddGameObject(prefabGenerator->CreateSphere(Vector3(70.0f, height + 50, - 90.0f), 1.0f, 0.2f, true));

		world->AddGameObject(prefabGenerator->CreateOBBCube(Vector3(70.0f, height + 100, -107.0f), Quaternion(), Vector3(1, 1, 1), 0.1f, true));
		world->AddGameObject(prefabGenerator->CreateOBBCube(Vector3(70.0f, height + 100, -100.0f), Quaternion(), Vector3(1, 1, 1), 0.1f, true));
		world->AddGameObject(prefabGenerator->CreateOBBCube(Vector3(70.0f, height + 100, - 93.0f), Quaternion(), Vector3(1, 1, 1), 0.1f, true));
	}
}

void Game::InitGauntlet2() {
	world->AddGameObject(prefabGenerator->CreateSphere(Vector3(100, 90, -100), 2.0f, 10.0f));
	world->AddGameObject(prefabGenerator->CreateSlipperyFloor(Vector3(118.75f, 86.35f, -55.0f), Quaternion(), Vector2(6.25f, 20.0f)));
	world->AddGameObject(prefabGenerator->CreateSlipperyFloor(Vector3(100.0f, 86.35f, -41.25f), Quaternion(), Vector2(12.5f, 6.25f)));
	world->AddGameObject(prefabGenerator->AddTreadmill(world, Vector3(81.25f, 86.75f, -26.25f), Quaternion(), 10.0f, Vector2(6.25f, 21.25f)));
}

//GameObject* Game::AddPlayerToWorld(const Vector3& position) {
//	float meshSize = 3.0f;
//	float inverseMass = 0.5f;
//
//	GameObject* character = new GameObject("player");
//
//	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.85f, 0.3f) * meshSize);
//
//	character->SetBoundingVolume((CollisionVolume*)volume);
//
//	character->GetTransform()
//		.SetScale(Vector3(meshSize, meshSize, meshSize))
//		.SetPosition(position);
//
//	if (rand() % 2) {
//		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshA, nullptr, basicShader));
//	}
//	else {
//		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshB, nullptr, basicShader));
//	}
//	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));
//
//	character->GetPhysicsObject()->SetInverseMass(inverseMass);
//	character->GetPhysicsObject()->InitSphereInertia();
//
//	world->AddGameObject(character);
//
//	//lockedObject = character;
//
//	return character;
//}

//GameObject* Game::AddEnemyToWorld(const Vector3& position) {
//	float meshSize		= 3.0f;
//	float inverseMass	= 0.5f;
//
//	GameObject* character = new GameObject("enemy");
//
//	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
//	character->SetBoundingVolume((CollisionVolume*)volume);
//
//	character->GetTransform()
//		.SetScale(Vector3(meshSize, meshSize, meshSize))
//		.SetPosition(position);
//
//	character->SetRenderObject(new RenderObject(&character->GetTransform(), enemyMesh, nullptr, basicShader));
//	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));
//
//	character->GetPhysicsObject()->SetInverseMass(inverseMass);
//	character->GetPhysicsObject()->InitSphereInertia();
//
//	world->AddGameObject(character);
//
//	return character;
//}

//GameObject* Game::AddBonusToWorld(const Vector3& position) {
//	GameObject* apple = new GameObject();
//
//	SphereVolume* volume = new SphereVolume(0.25f);
//	apple->SetBoundingVolume((CollisionVolume*)volume);
//	apple->GetTransform()
//		.SetScale(Vector3(0.25, 0.25, 0.25))
//		.SetPosition(position);
//
//	apple->SetRenderObject(new RenderObject(&apple->GetTransform(), bonusMesh, nullptr, basicShader));
//	apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));
//
//	apple->GetPhysicsObject()->SetInverseMass(1.0f);
//	apple->GetPhysicsObject()->InitSphereInertia();
//
//	world->AddGameObject(apple);
//
//	return apple;
//}

/*

Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be 
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around. 

*/
bool Game::SelectObject() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) {
		inSelectionMode = !inSelectionMode;
		if (inSelectionMode) {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (inSelectionMode) {
		renderer->DrawString("Press Q to change to camera mode!", Vector2(5, 85));

		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
			if (selectionObject) {	//set colour to deselected;
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));

				if(forwardObject)
					forwardObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));

				selectionObject = nullptr;
				forwardObject = nullptr;
				lockedObject	= nullptr;
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());
			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true)) {
				Debug::DrawLine(ray.GetPosition(), closestCollision.collidedAt, Vector4(0, 1, 0, 1), 10.0f);
				selectionObject = (GameObject*)closestCollision.node;
				selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				
				ray = Ray(selectionObject->GetTransform().GetPosition(), selectionObject->GetTransform().GetOrientation() * Vector3(0, 0, -1));
				if (world->Raycast(ray, closestCollision, true)) {
					Debug::DrawLine(ray.GetPosition(), closestCollision.collidedAt, Vector4(1, 1, 0, 1), 10.0f);
					forwardObject = (GameObject*)closestCollision.node;
					forwardObject->GetRenderObject()->SetColour(Vector4(1, 1, 0, 1));
				}
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		renderer->DrawString("Press Q to change to select mode!", Vector2(5, 85));
	}

	if (lockedObject) {
		renderer->DrawString("Press L to unlock object!", Vector2(5, 80));
	}

	else if(selectionObject){
		renderer->DrawString("Press L to lock selected object object!", Vector2(5, 80));
	}

	if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::L)) {
		if (selectionObject) {
			if (lockedObject == selectionObject) {
				lockedObject = nullptr;
			}
			else {
				lockedObject = selectionObject;
			}
		}

	}

	return false;
}

/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/
void Game::MoveSelectedObject() {
	renderer->DrawString("Click Force: " + std::to_string(forceMagnitude), Vector2(10, 20));
	forceMagnitude += Window::GetMouse()->GetWheelMovement() * 100.0f;

	if (!selectionObject) {
		return;
	}

	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::RIGHT)) {
		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());
		RayCollision closestCollision;

		if (world->Raycast(ray, closestCollision, true)) {
			if (closestCollision.node == selectionObject) {
				selectionObject->GetPhysicsObject()->AddForceAtPosition(ray.GetDirection() * forceMagnitude, closestCollision.collidedAt);
			}
		}
	}

	if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::UP)) {
		selectionObject->GetPhysicsObject()->AddForce(Vector3(0,0,-1) * forceMagnitude);
	}

	if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::DOWN)) {
		selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 1) * forceMagnitude);
	}

	if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::LEFT)) {
		selectionObject->GetPhysicsObject()->AddForce(Vector3(-1, 0, 0) * forceMagnitude);
	}

	if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::RIGHT)) {
		selectionObject->GetPhysicsObject()->AddForce(Vector3(1, 0, 0) * forceMagnitude);
	}

}