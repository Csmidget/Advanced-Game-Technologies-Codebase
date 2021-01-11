#include "NavigationGrid.h"
#include "../../Common/Assets.h"

#include <fstream>
#include <set>

using namespace NCL;
using namespace CSC8503;

const int LEFT_NODE		= 0;
const int RIGHT_NODE	= 1;
const int TOP_NODE		= 2;
const int BOTTOM_NODE	= 3;

const char WALL_NODE	= 'x';
const char FLOOR_NODE	= '.';

NavigationGrid::NavigationGrid()	{
	nodeSize	= 0;
	gridWidth	= 0;
	gridHeight	= 0;
	allNodes	= nullptr;
}

NavigationGrid::NavigationGrid(const std::string&filename, Vector3 offset) : NavigationGrid() {
	std::ifstream infile(Assets::DATADIR + filename);
	this->offset = offset;
	infile >> nodeSize;
	infile >> gridWidth;
	infile >> gridHeight;

	allNodes = new GridNode[gridWidth * gridHeight];

	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			GridNode&n = allNodes[(gridWidth * y) + x];
			char type = 0;
			infile >> type;
			n.type = type;
			n.position = Vector3((float)(x * nodeSize), 0, (float)(y * nodeSize));
		}
	}
	
	//now to build the connectivity between the nodes
	for (int y = 0; y < gridHeight; ++y) {
		for (int x = 0; x < gridWidth; ++x) {
			GridNode&n = allNodes[(gridWidth * y) + x];		

			if (y > 0) { //get the above node

				n.connected[0] = &allNodes[(gridWidth * (y - 1)) + x];

				if (x > 0) {
					n.connected[4] = &allNodes[(gridWidth * (y - 1)) + x - 1];
				}

				if (x < gridWidth - 1) {
					n.connected[5] = &allNodes[(gridWidth * (y - 1)) + x + 1];
				}

			}
			if (y < gridHeight - 1) { //get the below node

				n.connected[1] = &allNodes[(gridWidth * (y + 1)) + x];

				if (x > 0) {
					n.connected[6] = &allNodes[(gridWidth * (y + 1)) + x - 1];
				}

				if (x < gridWidth - 1) {
					n.connected[7] = &allNodes[(gridWidth * (y + 1)) + x + 1];
				}
			}

			if (x > 0) { //get left node
				n.connected[2] = &allNodes[(gridWidth * (y)) + (x - 1)];
			}
			if (x < gridWidth - 1) { //get right node
				n.connected[3] = &allNodes[(gridWidth * (y)) + (x + 1)];
			}

			//diagonals connected to each of the cardinal directions. e.g. left is connected to up-left and down-left.
			std::pair<int, int> diagonalNeighbours[4]{ {4,5},{6,7},{4,6},{5,7} };

			for (int i = 0; i < 4; ++i) {
				if (n.connected[i]) {
					if (n.connected[i]->type == '.') {
						n.costs[i]		= 1;
					}
					if (n.connected[i]->type == 'x') {
						n.connected[i] = nullptr; //actually a wall, disconnect!

						//Also disconnect diagonals connected to this neighbour, to avoid ai ramming themselves into walls
						n.connected[diagonalNeighbours[i].first] = nullptr;
						n.connected[diagonalNeighbours[i].second] = nullptr;
					}
				}
			}

			for (int i = 4; i < 8; ++i) {
				if (n.connected[i]) {
					if (n.connected[i]->type == '.') {
						n.costs[i] = 1.414214f;
					}
					if (n.connected[i]->type == 'x') {
						n.connected[i] = nullptr; //actually a wall, disconnect!
					}
				}
			}
		}	
	}
}

NavigationGrid::~NavigationGrid()	{
	delete[] allNodes;
}

bool NavigationGrid::FindPath(const Vector3& rawFrom, const Vector3& rawTo, NavigationPath& outPath) {

	Vector3 from = rawFrom + offset;
	Vector3 to   = rawTo + offset;

	//need to work out which node 'from' sits in, and 'to' sits in
	int fromX = (round(from.x / nodeSize));
	int fromZ = (round(from.z / nodeSize));

	//Round before casting to ensure closest node (cast to int will floor the value)
	int toX = (round(to.x / nodeSize));
	int toZ = (round(to.z / nodeSize));

	if (fromX < 0 || fromX > gridWidth - 1 ||
		fromZ < 0 || fromZ > gridHeight - 1) {
		return false; //outside of map region!
	}

	if (toX < 0 || toX > gridWidth - 1 ||
		toZ < 0 || toZ > gridHeight - 1) {
		return false; //outside of map region!
	}

	GridNode* startNode = &allNodes[(fromZ * gridWidth) + fromX];
	GridNode* endNode	= &allNodes[(toZ * gridWidth) + toX];
	
	NodeSet openSet;
	NodeSet closedSet;

	openSet.emplace(startNode);

	startNode->f = 0;
	startNode->g = 0;
	startNode->parent = nullptr;

	GridNode* currentBestNode = nullptr;

	while (!openSet.empty()) {
		currentBestNode = *openSet.begin();
		openSet.erase(openSet.begin());

		if (currentBestNode == endNode) {			//we've found the path!
			GridNode* node = endNode;
			outPath.PushWaypoint(rawTo);
			while (node != nullptr) {
				outPath.PushWaypoint(node->position - offset);
				node = node->parent;
			}
			return true;
		}
		else {
			for (int i = 0; i < 8; ++i) {
				GridNode* neighbour = currentBestNode->connected[i];
				if (!neighbour) { //might not be connected...
					continue;
				}	
				bool inClosed	= NodeInSet(neighbour, closedSet);
				if (inClosed) {
					continue; //already discarded this neighbour...
				}

				float h = Heuristic(neighbour, endNode);				
				float g = currentBestNode->g + currentBestNode->costs[i];
				float f = h + g;

				bool inOpen = NodeInSet(neighbour, openSet);

				if (inOpen && f < neighbour->f) {
					openSet.erase(neighbour);
					inOpen = false;
				}

				if (!inOpen) {//might be a better route to this neighbour
					neighbour->parent = currentBestNode;
					neighbour->f = f;
					neighbour->g = g;
				}

				if (!inOpen) {
					openSet.emplace(neighbour);
				}
			}
			closedSet.emplace(currentBestNode);
		}
	}
	return false; //open list emptied out with no path!
}

bool NavigationGrid::NodeInSet(GridNode* n, NodeSet& set) const {
	NodeSet::iterator i = set.find(n);
	return i == set.end() ? false : true;
}

float NavigationGrid::Heuristic(GridNode* hNode, GridNode* endNode) const {
	return (hNode->position - endNode->position).Length();
}