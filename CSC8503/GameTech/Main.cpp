#include "../../Common/Window.h"

#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/State.h"

#include "../CSC8503Common/NavigationGrid.h"
#include "../CSC8503Common/PushdownState.h"
#include "../CSC8503Common/PushdownMachine.h"

#include "../CSC8503Common/BehaviourSequence.h"
#include "../CSC8503Common/BehaviourAction.h"

#include "TutorialGame.h"
#include "../CSC8503Common/BehaviourSelector.h"
#include "../CSC8503Common/BehaviourParallel.h"
#include "../CSC8503Common/BehaviourDecorator.h"

using namespace NCL;
using namespace CSC8503;

vector<Vector3> testNodes;
void TestPathfinding() {
	NavigationGrid grid("TestGrid1.txt");
	NavigationPath outPath;
	
	Vector3 startPos(160, 0, 20);
	Vector3 endPos(160, 0, 160);

	bool found = grid.FindPath(startPos, endPos, outPath);

	Vector3 pos;
	while (outPath.PopWaypoint(pos)) {
		testNodes.push_back(pos);
	};
}

void DisplayPathfinding() {
	for (int i = 1; i < testNodes.size(); ++i) {
		Vector3 a = testNodes[i - 1] + Vector3(0,1,0);
		Vector3 b = testNodes[i] + Vector3(0, 1, 0);

		Debug::DrawLine(a, b, Vector4(0, 1, 0, 1));
	}
}

void TestStateMachine() {
	StateMachine* testMachine = new StateMachine();
	int data = 0;

	State* A = new State([&](float dt)->void 
		{
			std::cout << "I'm in state A\n";
			data++;
		});

	State* B = new State([&](float dt)->void
		{
			std::cout << "I'm in state B!\n";
			data--;
		});

	StateTransition* stateAB = new StateTransition(A, B, [&](void)->bool
		{
			return data > 10;
		});

	StateTransition* stateBA = new StateTransition(B, A, [&](void)->bool
		{
			return data < 0;
		});

	testMachine->AddState(A);
	testMachine->AddState(B);
	testMachine->AddTransition(stateAB);
	testMachine->AddTransition(stateBA);

	for (int i = 0; i < 100; ++i) {
		testMachine->Update(0.0f);
	}
}

class PauseScreen : public PushdownState {
	PushdownResult OnUpdate(float dt, PushdownState** newState) override {
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::U)) {
			return PushdownResult::Pop;
		}
		return PushdownResult::NoChange;
	}

	void OnAwake() override {
		std::cout << "Press U to unpause game!\n";
	}
};

class GameScreen : public PushdownState {
	PushdownResult OnUpdate(float dt, PushdownState** newState) override {
		pauseReminder -= dt;

		if (pauseReminder < 0) {
			std::cout << "Coins mined:" << coinsMined << "\n";
			std::cout << "Press P to pause game, of F1 to return to main menu!\n";
			pauseReminder += 1.0f;
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::P)) {
			*newState = new PauseScreen();
			return PushdownResult::Push;
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F1)) {
			std::cout << "Returning to main menu!\n";
			return PushdownResult::Pop;
		}
		if (rand() % 7 == 0) {
			coinsMined++;
		}
		return PushdownResult::NoChange;
	};

	void OnAwake() override {
		std::cout << "Preparing to mine coins!\n";
	}

	protected:
		int coinsMined = 0;
		float pauseReminder = 1;
};

class IntroScreen : public PushdownState {
	PushdownResult OnUpdate(float dt, PushdownState** newState) override {
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
			*newState = new GameScreen();
			return PushdownResult::Push;
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::ESCAPE)) {
			return PushdownResult::Pop;
		}
		return PushdownResult::NoChange;
	}

	void OnAwake() override {
		std::cout << "Welcome to a really awesome game!\n";
		std::cout << "Press Space to Begin or escape to quit!\n";
	}
};

void TestPushdownAutomata(Window* w) {
	PushdownMachine machine(new IntroScreen());
	while (w->UpdateWindow()) {
		float dt = w->GetTimer()->GetTimeDeltaSeconds();
		if (!machine.Update(dt)) {
			return;
		}
	}
}

void TestBehaviourTree() {
	float behaviourTimer;
	float distanceToTarget;

	BehaviourAction* findKey = new BehaviourAction("Find Key", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == BehaviourState::Initialise) {
			std::cout << "Looking for a key!\n";
			behaviourTimer = (float)(rand() % 100);
			state = BehaviourState::Ongoing;
		}
		else if (state == BehaviourState::Ongoing) {
			behaviourTimer -= dt;
			if (behaviourTimer <= 0.0f) {
				std::cout << "Found a key!\n";
				return BehaviourState::Success;
			}
		}
		return state;
	});

	BehaviourAction* goToRoom = new BehaviourAction("Go to room", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == BehaviourState::Initialise) {
			std::cout << "Going to the loot room!\n";
			state = BehaviourState::Ongoing;
		}
		else if (state == BehaviourState::Ongoing) {
			distanceToTarget -= dt;
			if (distanceToTarget <= 0.0f) {
				std::cout << "Reached room!\n";
				return BehaviourState::Success;
			}
		}
		return state;
	});

	BehaviourAction* openDoor = new BehaviourAction("Open Door", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == BehaviourState::Initialise) {
			std::cout << "Opening Door!\n";
			return BehaviourState::Success;
		}
		return state;
	});

	BehaviourAction* lookForTreasure = new BehaviourAction("Look For Treasure", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == BehaviourState::Initialise) {
			std::cout << "Looking for treasure!\n";
			return BehaviourState::Ongoing;
		}
		else if (state == BehaviourState::Ongoing) {
			bool found = rand() % 2;
			if (found) {
				std::cout << "I found some treasure!\n";
				return BehaviourState::Success;
			}
			std::cout << "No treasure in here...\n";
			return BehaviourState::Failure;
		}
		return state;
		});

	BehaviourAction* lookForItems = new BehaviourAction("Look For Items", [&](float dt, BehaviourState state)->BehaviourState {
		if (state == BehaviourState::Initialise) {
			std::cout << "Looking for items!\n";
			return BehaviourState::Ongoing;
		}
		else if (state == BehaviourState::Ongoing) {
			bool found = rand() % 2;
			if (found) {
				std::cout << "I found some items!\n";
				return BehaviourState::Success;
			}
			std::cout << "No items in here...\n";
			return BehaviourState::Failure;
		}
		return state;
	});

	BehaviourSequence* sequence =
		new BehaviourSequence("Room Sequence");
	sequence->AddChild(findKey);
	sequence->AddChild(goToRoom);
	sequence->AddChild(openDoor);

	BehaviourParallel* selection =
		new BehaviourParallel("Loot Selection");
	selection->AddChild(lookForTreasure);
	selection->AddChild(lookForItems);

	BehaviourDecorator* inverter = new BehaviourDecorator("Inverter", [](BehaviourState state)->BehaviourState {
		switch (state) {
			case BehaviourState::Failure:
				return BehaviourState::Success;
			case BehaviourState::Success:
				return BehaviourState::Failure;
			default:
				return state;
		}
	});
	inverter->SetChild(selection);

	BehaviourSequence* rootSequence =
		new BehaviourSequence("Root Sequence");
	rootSequence->AddChild(sequence);
	rootSequence->AddChild(inverter);

	for (int i = 0; i < 5; ++i) {
		rootSequence->Reset();
		behaviourTimer = 0.0f;
		distanceToTarget = (float)(rand() % 250);
		BehaviourState state = BehaviourState::Ongoing;
		std::cout << "We're going on an adventure!\n";
		
		while (state == BehaviourState::Ongoing) {
			state = rootSequence->Execute(1.0f);
			Sleep(10);
		}
		if (state == BehaviourState::Success) {
			std::cout << "What a successful adventure!\n";
		}
		else if (state == BehaviourState::Failure) {
			std::cout << "What a waste of time!\n";
		}	
	}
	std::cout << "All done!\n";
}



/*

The main function should look pretty familar to you!
We make a window, and then go into a while loop that repeatedly
runs our 'game' until we press escape. Instead of making a 'renderer'
and updating it, we instead make a whole game, and repeatedly update that,
instead. 

This time, we've added some extra functionality to the window class - we can
hide or show the 

*/
int main() {
	Window*w = Window::CreateGameWindow("CSC8503 Game technology!", 1280, 720);

    TestPathfinding();
//	TestBehaviourTree();
	//TestStateMachine();
	//TestPushdownAutomata(w);
	if (!w->HasInitialised()) {
		return -1;
	}	
	srand((unsigned int)time(0));
	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);

	TutorialGame* g = new TutorialGame();
	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!
	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		DisplayPathfinding();
		float dt = w->GetTimer()->GetTimeDeltaSeconds();
		if (dt > 0.1f) {
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::PRIOR)) {
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NEXT)) {
			w->ShowConsole(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T)) {
			w->SetWindowPosition(0, 0);
		}

		w->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));

		g->UpdateGame(dt);
	}
	Window::DestroyGameWindow();
}