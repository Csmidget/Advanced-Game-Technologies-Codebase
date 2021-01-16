#include "Checkpoint.h"
#include "../CSC8503Common/GameObject.h"
#include "../CSC8503Common/CollisionDetection.cpp"

using namespace NCL;
using namespace CSC8503;

//Checkpoint collision doesn't have to be too precise, just use the objects broadphase AABB.
bool Checkpoint::ReachedCheckpoint(const GameObject* object) const {

	Vector3 objectAABB;
	if (object && object->GetBroadphaseAABB(objectAABB)) {
		return CollisionDetection::AABBTest(object->GetTransform().GetPosition(), position, objectAABB, halfDims);
	}

	return false;
}