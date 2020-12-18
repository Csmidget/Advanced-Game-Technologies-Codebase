#include "StateGameObject.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/State.h"

using namespace NCL;
using namespace CSC8503;

StateGameObject::StateGameObject() {
	counter = 0.0f;
	stateMachine = new StateMachine();

	State* stateA = new GenericState([](float dt, void* data)->void {
		((StateGameObject*)data)->MoveLeft(dt);
		},this);

	State* stateB = new GenericState([](float dt, void* data)->void {
		((StateGameObject*)data)->MoveRight(dt);
		}, this);

	stateMachine->AddState(stateA);
	stateMachine->AddState(stateB);

	stateMachine->AddTransition(new GenericTransition<float*, void*>([](float* counter, void*)->bool {
			return (*(float*)counter) > 3.0f;
		},&counter,nullptr,stateA,stateB));

	stateMachine->AddTransition(new GenericTransition<float*, void*>([](float* counter, void*)->bool {
		return (*(float*)counter) < 0.0f;
		}, &counter, nullptr, stateB, stateA));
}

StateGameObject::~StateGameObject() {
	delete stateMachine;
}

void StateGameObject::Update(float dt) {
	stateMachine->Update(dt);
}

void StateGameObject::MoveLeft(float dt) {
	GetPhysicsObject()->AddForce({ -10,0,0 });
	counter += dt;
}

void StateGameObject::MoveRight(float dt) {
	GetPhysicsObject()->AddForce({ 10,0,0 });
	counter -= dt;
}