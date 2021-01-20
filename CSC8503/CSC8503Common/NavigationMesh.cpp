#include "NavigationMesh.h"
#include "../../Common/Assets.h"
#include "../../Common/Maths.h"
#include <fstream>
using namespace NCL;
using namespace CSC8503;
using namespace std;

NavigationMesh::NavigationMesh()
{
}

NavigationMesh::NavigationMesh(const std::string& filename)
{
	ifstream file(Assets::DATADIR + filename);

	int numVertices = 0;
	int numIndices = 0;

	file >> numVertices;
	file >> numIndices;

	for (int i = 0; i < numVertices; ++i) {
		Vector3 vert;
		file >> vert.x;
		file >> vert.y;
		file >> vert.z;

		allVerts.emplace_back(vert);
	}

	allTris.resize(numIndices / 3);

	for (int i = 0; i < allTris.size(); ++i) {
		NavTri* tri = &allTris[i];
		file >> tri->indices[0];
		file >> tri->indices[1];
		file >> tri->indices[2];

		tri->centroid = allVerts[tri->indices[0]] +
			allVerts[tri->indices[1]] +
			allVerts[tri->indices[2]];

		tri->centroid = allTris[i].centroid / 3.0f;

		tri->triPlane = Plane::PlaneFromTri(allVerts[tri->indices[0]],
			allVerts[tri->indices[1]],
			allVerts[tri->indices[2]]);

		tri->area = Maths::CrossAreaOfTri(allVerts[tri->indices[0]], allVerts[tri->indices[1]], allVerts[tri->indices[2]]);
	}
	for (int i = 0; i < allTris.size(); ++i) {
		NavTri* tri = &allTris[i];
		for (int j = 0; j < 3; ++j) {
			int index = 0;
			file >> index;
			if (index != -1) {
				tri->neighbours[j] = &allTris[index];
			}
		}
	}
}

NavigationMesh::~NavigationMesh()
{
}


bool NavigationMesh::FindPath(const Vector3& from, const Vector3& to, NavigationPath& outPath, float maximumCost) {

	const NavTri* start = GetTriForPosition(from);
	const NavTri* end = GetTriForPosition(to);

	NodeSet openSet;
	NodeSet closedSet;

	PathNode* startNode = new PathNode();
	startNode->f = 0.0f;
	startNode->g = 0.0f;
	startNode->parent = nullptr;
	startNode->tri = start;


	openSet.emplace(startNode);

	PathNode* currentBestNode = nullptr;

	bool endFound = false;

	while (!endFound && !openSet.empty()) {
		currentBestNode = *openSet.begin();
		openSet.erase(openSet.begin());

		if (currentBestNode->tri == end) { //we've found the path!
			endFound = true;
		}
		else {
			for (int i = 0; i < 3; ++i) {

				if (!currentBestNode->tri->neighbours[i]) {
					continue;
				}

				PathNode* neighbour = new PathNode();
				neighbour->tri = currentBestNode->tri->neighbours[i];
				neighbour->parent = currentBestNode;

				bool inClosed = NodeInSet(neighbour, closedSet) != nullptr;
				if (inClosed) {
					continue; //already discarded this neighbour...
				}

				float h = (neighbour->tri->centroid - end->centroid).Length();
				float g = currentBestNode->g + (neighbour->tri->centroid - currentBestNode->tri->centroid).Length();
				float f = h + g;

				PathNode* openSetNode = NodeInSet(neighbour, openSet);

				if (openSetNode && f < openSetNode->f) {
					openSet.erase(openSetNode);
					delete openSetNode;
					openSetNode = nullptr;
				}
				
				if (!openSetNode) {//might be a better route to this neighbour
					neighbour->h = h;
					neighbour->f = f;
					neighbour->g = g;
				}

				if (!openSetNode && neighbour->g < maximumCost) {
					openSet.emplace(neighbour);
				}
			}
		}
		closedSet.emplace(currentBestNode);
	}

	if (endFound) {
		PathNode* node = currentBestNode;
		outPath.PushWaypoint(to);

		while (node != nullptr) {
			outPath.PushWaypoint(node->tri->centroid);
			node = node->parent;
		}
	}

	for (auto node : closedSet) {
		delete node;
	}
	for (auto node : openSet) {
		delete node;
	}

	return false; //open list emptied out with no path!
}

PathNode* NavigationMesh::NodeInSet(PathNode* n, NodeSet& set) const {

	NodeSet::iterator i = std::find_if(set.begin(),set.end(), [&n](PathNode* a) {
		return a->tri == n->tri;
	});

	return i == set.end() ? nullptr : *i;
}

/*
If you have triangles on top of triangles in a full 3D environment, you'll need to change this slightly,
as it is currently ignoring height. You might find tri/plane raycasting is handy.
*/

const NavTri* NavigationMesh::GetTriForPosition(const Vector3& pos) const {
//	Vector3 testPos = pos;
//	testPos.z = -testPos.z;

	for (const NavTri& t : allTris) {
		Vector3 planePoint = t.triPlane.ProjectPointOntoPlane(pos);

		float ta = Maths::CrossAreaOfTri(allVerts[t.indices[0]], allVerts[t.indices[1]], planePoint);
		float tb = Maths::CrossAreaOfTri(allVerts[t.indices[1]], allVerts[t.indices[2]], planePoint);
		float tc = Maths::CrossAreaOfTri(allVerts[t.indices[2]], allVerts[t.indices[0]], planePoint);

		float areaSum = ta + tb + tc;

		if (abs(areaSum - t.area) > 0.001f) { //floating points are annoying! Are we more or less inside the triangle?
			continue;
		}
		return &t;
	}
	return nullptr;
}