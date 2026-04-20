#include "AABB_Tree_Dynamic.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>

void CollisionSystem2D::AABB_Tree_Dynamic::createNewBox(int id, Point2D min, Point2D max)
{
	AABB newBox;
	newBox.uniqueID = id;
	newBox.min = min;
	newBox.max = max;
	uniqueIDtoBoxIndexMap[id] = boxes.size();
	boxIndexToUniqueIDMap[boxes.size()] = id;
	boxes.push_back(newBox);
	boxRegions.push_back(newBox);
	expandBox(boxRegions[boxes.size() - 1], 0.2f);
}

void CollisionSystem2D::AABB_Tree_Dynamic::constructTreeFromExisting() {
	std::vector<int> ids;
	for (size_t i = 0; i < boxes.size(); i++) { ids.push_back(i); }
	root = createBranch(ids.begin(), ids.size(), nullptr);
}

CollisionSystem2D::AABB CollisionSystem2D::AABB_Tree_Dynamic::getBox(int uniqueID)
{
	int boxIndex{ uniqueIDtoBoxIndexMap[uniqueID] };
	return boxes[boxIndex];
}

CollisionSystem2D::AABB_Tree_Dynamic_Node* CollisionSystem2D::AABB_Tree_Dynamic::createBranch(
													std::vector<int>::iterator begin, int size, AABB_Tree_Dynamic_Node* parent) {
	if (size == 0) return nullptr;

	if (size == 1) {
		int boxIndex = *begin;
		AABB& box = boxes[boxIndex];

		AABB_Tree_Dynamic_Node* leafNode = new AABB_Tree_Dynamic_Node();
		leafNode->boxIndex = boxIndex;
		leafNode->nodeBox.min = box.min;
		leafNode->nodeBox.max = box.max;
		leafNode->nodeBox.uniqueID = boxIndexToUniqueIDMap[boxIndex];
		leafNode->left = nullptr;
		leafNode->right = nullptr;
		leafNode->parent = parent;

		expandBox(leafNode->nodeBox, 0.2f);

		boxIndexToLeafNodeMap[boxIndex] = leafNode;

		return leafNode;
	}

	Point2D collectiveMin, collectiveMax;
	char axisOfSplit = getAxisOfSplit(begin, size, collectiveMin, collectiveMax);
	partitionByAxis(begin, size, axisOfSplit);
	int leftBranchSize{ size / 2 };
	int rightBranchSize{ size - leftBranchSize };

	AABB_Tree_Dynamic_Node* internalNode = new AABB_Tree_Dynamic_Node();
	internalNode->left = createBranch(begin, leftBranchSize, internalNode);
	internalNode->right = createBranch(begin + leftBranchSize, rightBranchSize, internalNode);
	internalNode->parent = parent;
	internalNode->nodeBox.min = collectiveMin;
	internalNode->nodeBox.max = collectiveMax;
	internalNode->boxIndex = -1;
	return internalNode;
}

char CollisionSystem2D::AABB_Tree_Dynamic::getAxisOfSplit(std::vector<int>::iterator begin, int size, Point2D& collectiveMin, Point2D& collectiveMax)
{
	int first = *begin;

	collectiveMin.x = boxRegions[first].min.x;
	collectiveMin.y = boxRegions[first].min.y;
	collectiveMax.x = boxRegions[first].max.x;
	collectiveMax.y = boxRegions[first].max.y;

	for (std::vector<int>::iterator itr = begin + 1; itr < begin + size; itr++) {
		AABB& leaf = boxRegions[*itr];
		if (leaf.min.x < collectiveMin.x) collectiveMin.x = leaf.min.x;
		if (leaf.min.y < collectiveMin.y) collectiveMin.y = leaf.min.y;
		if (leaf.max.x > collectiveMax.x) collectiveMax.x = leaf.max.x;
		if (leaf.max.y > collectiveMax.y) collectiveMax.y = leaf.max.y;
	}

	float distanceX{ collectiveMax.x - collectiveMin.x };
	float distanceY{ collectiveMax.y - collectiveMin.y };

	if (distanceY > distanceX) return 'Y';

	return 'X';
}

void CollisionSystem2D::AABB_Tree_Dynamic::partitionByAxis(std::vector<int>::iterator begin, int size, char axisOfSplit)
{

	if (axisOfSplit == 'Y') {

		std::nth_element(begin, begin + (size / 2), begin + size, [this](const int& A, const int& B) {
			float centerA = this->boxRegions[A].min.y + ((this->boxRegions[A].max.y - this->boxRegions[A].min.y) / 2);
			float centerB = this->boxRegions[B].min.y + ((this->boxRegions[B].max.y - this->boxRegions[B].min.y) / 2);
			return centerA < centerB;
			});

		return;
	}

	std::nth_element(begin, begin + (size / 2), begin + size, [this](const int& A, const int& B) {
		float centerA = this->boxRegions[A].min.x + ((this->boxRegions[A].max.x - this->boxRegions[A].min.x) / 2);
		float centerB = this->boxRegions[B].min.x + ((this->boxRegions[B].max.x - this->boxRegions[B].min.x) / 2);
		return centerA < centerB;
		});
}

void CollisionSystem2D::AABB_Tree_Dynamic::expandBox(AABB& box, float percentage)
{
	float boxWidth{ box.max.x - box.min.x };
	float expandedWidth{ boxWidth * (1 + percentage) };
	float marginX = (expandedWidth - boxWidth) / 2;	
	box.min.x -= marginX;
	box.max.x += marginX;

	float boxHeight{ box.max.y - box.min.y };
	float expandedHeight{ boxHeight * (1 + percentage) };
	float marginY = (expandedHeight - boxHeight) / 2;
	box.min.y -= marginY;
	box.max.y += marginY;
}

bool CollisionSystem2D::AABB_Tree_Dynamic::overlaps(AABB* A, AABB* B)
{
	bool noCollisionX = A->max.x < B->min.x || B->max.x < A->min.x;

	if (noCollisionX) return false;

	bool noCollisionY = A->max.y < B->min.y || B->max.y < A->min.y;

	if (noCollisionY) return false;

	return true;

}

void CollisionSystem2D::AABB_Tree_Dynamic::updateBox(AABB* newBox)
{
	int boxIndex{ uniqueIDtoBoxIndexMap[newBox->uniqueID] };
	AABB& box{ boxes[boxIndex] };
	box = *newBox;

	AABB_Tree_Dynamic_Node*& leafNode{ boxIndexToLeafNodeMap[boxIndex] };

	if (leafNode->nodeBox.min.x <= box.min.x && leafNode->nodeBox.max.x >= box.max.x
		&& leafNode->nodeBox.min.y <= box.min.y && leafNode->nodeBox.max.y >= box.max.y) {
		return;
	}

	leafNode->nodeBox = boxes[boxIndex];
	expandBox(leafNode->nodeBox, 0.2f);

	reinsertNode(leafNode);
}

void CollisionSystem2D::AABB_Tree_Dynamic::reinsertNode(AABB_Tree_Dynamic_Node* node)
{

	AABB_Tree_Dynamic_Node* parent = node->parent;

	if (parent == nullptr) {
		return;
	}

	AABB_Tree_Dynamic_Node* sibling = nullptr;

	if (parent->left != nullptr && parent->left != node) {
		sibling = parent->left;
	}
	else if (parent->right != nullptr && parent->right != node) {
		sibling = parent->right;
	}

	AABB_Tree_Dynamic_Node* grandParent = parent->parent;

	if (grandParent == nullptr) {
		root = sibling;
	}
	else {
		if (grandParent->left == parent) grandParent->left = sibling;
		if (grandParent->right == parent) grandParent->right = sibling;
	}

	if (sibling == nullptr) {
		std::cout << "error: sibling is null" << std::endl;
		exit(0);
	}

	sibling->parent = grandParent;

	delete parent;
	parent = nullptr;

	node->parent = nullptr;

	insert(node, root);
	fixRegion(node);
	fixRegion(sibling);

}



void CollisionSystem2D::AABB_Tree_Dynamic::insert(AABB_Tree_Dynamic_Node* node, AABB_Tree_Dynamic_Node* at)
{
	if (at == nullptr) {
		root = node;
		return;
	}

	if (at->left == nullptr && at->right == nullptr) {
		insertSibling(node, at);
		return;
	}

	if (at->left == nullptr || at->right == nullptr) {
		std::cout << "error: each internal node in an AABB tree should have two children";
		exit(0);
	}

	float regionEnlargementLeft = getRegionEnlargement(node, at->left);
	float regionEnlargementRight = getRegionEnlargement(node, at->right);

	if (regionEnlargementLeft < regionEnlargementRight) {
		insert(node, at->left);
	}
	else {
		insert(node, at->right);
	}
}

float CollisionSystem2D::AABB_Tree_Dynamic::getRegionEnlargement(AABB_Tree_Dynamic_Node* node, AABB_Tree_Dynamic_Node* other)
{
	AABB& newNodeBox = node->nodeBox;
	AABB& otherBox = other->nodeBox;
	AABB newBox;
	newBox.min.x = std::min(newNodeBox.min.x, otherBox.min.x);
	newBox.min.y = std::min(newNodeBox.min.y, otherBox.min.y);
	newBox.max.x = std::max(newNodeBox.max.x, otherBox.max.x);
	newBox.max.y = std::max(newNodeBox.max.y, otherBox.max.y);

	float newBoxArea = (newBox.max.x - newBox.min.x) * (newBox.max.y - newBox.min.y);
	float leftBoxArea = (otherBox.max.x - otherBox.min.x) * (otherBox.max.y - otherBox.min.y);
	return newBoxArea - leftBoxArea;
}

void CollisionSystem2D::AABB_Tree_Dynamic::insertSibling(AABB_Tree_Dynamic_Node* adoptedChild, AABB_Tree_Dynamic_Node* biologicalChild)
{
	AABB_Tree_Dynamic_Node* parent = biologicalChild->parent;

	AABB_Tree_Dynamic_Node* stepParent = new AABB_Tree_Dynamic_Node();
	stepParent->left = adoptedChild;
	stepParent->right = biologicalChild;
	stepParent->parent = parent;
	adoptedChild->parent = stepParent;
	biologicalChild->parent = stepParent;

	if (root == biologicalChild) {
		root = stepParent;
		stepParent->parent = nullptr;
	}
	else if (parent->left == biologicalChild) {
		parent->left = stepParent;
	}
	else if (parent->right == biologicalChild) {
		parent->right = stepParent;
	}
}

void CollisionSystem2D::AABB_Tree_Dynamic::fixRegion(AABB_Tree_Dynamic_Node* node)
{
	if (node == nullptr) {
		return;
	}

	AABB_Tree_Dynamic_Node* left = node->left;
	AABB_Tree_Dynamic_Node* right = node->right;

	if (left == nullptr || right == nullptr) {
		fixRegion(node->parent);
		return;
	}

	node->nodeBox.min.x = std::min(left->nodeBox.min.x, right->nodeBox.min.x);
	node->nodeBox.min.y = std::min(left->nodeBox.min.y, right->nodeBox.min.y);
	node->nodeBox.max.x = std::max(left->nodeBox.max.x, right->nodeBox.max.x);
	node->nodeBox.max.y = std::max(left->nodeBox.max.y, right->nodeBox.max.y);

	fixRegion(node->parent);
	
}

void CollisionSystem2D::AABB_Tree_Dynamic::destroyNode(AABB_Tree_Dynamic_Node* node)
{
	if (node == nullptr) return;

	if (node->left != nullptr) {
		destroyNode(node->left);
	}

	if (node->right != nullptr) {
		destroyNode(node->right);
	}

	delete node;
}

void CollisionSystem2D::AABB_Tree_Dynamic::attemptToMove(int uniqueID, AABB* newBox, bool& didMove, std::unordered_set<int>& collisionIDs)
{
	getCollisions(newBox, collisionIDs, root);

	didMove = collisionIDs.empty();

	if (!didMove) return;

	updateBox(newBox);
}

void CollisionSystem2D::AABB_Tree_Dynamic::forceMove(AABB* newBox)
{
	updateBox(newBox);
}


void CollisionSystem2D::AABB_Tree_Dynamic::getCollisions(AABB* box, std::unordered_set<int>& collisions) {
	getCollisions(box, collisions, root);
}

void CollisionSystem2D::AABB_Tree_Dynamic::getCollisions(AABB* box, std::unordered_set<int>& collisions, AABB_Tree_Dynamic_Node* node)
{
	if (node == nullptr) {
		return;
	}

	if (node->left != nullptr) {
		if (overlaps(&node->left->nodeBox, box)) {
			getCollisions(box, collisions, node->left);
		}
	}
	if (node->right != nullptr) {
		if (overlaps(&node->right->nodeBox, box)) {
			getCollisions(box, collisions, node->right);
		}
	}

	if (node->right == nullptr && node->left == nullptr) {
		AABB* other = &boxes[node->boxIndex];

		if (box->uniqueID == other->uniqueID) {
			return;
		}

		if (overlaps(other, box)) {
			int id = boxIndexToUniqueIDMap[node->boxIndex];
			collisions.insert(id);
		}
	}
}

CollisionSystem2D::AABB_Tree_Dynamic::~AABB_Tree_Dynamic()
{
	destroyNode(root);
}

void CollisionSystem2D::AABB_Tree_Dynamic::print(AABB_Tree_Dynamic_Node* node) {
	if (node == nullptr) {
		return;
	}

	AABB box = node->nodeBox;
	std::cout << "Index " << node->boxIndex << " BOX " << box.uniqueID << " : " << box.min.x << " " << box.min.y << " " << box.min.y << " " << box.max.y << std::endl;

	AABB_Tree_Dynamic_Node* parent = node->parent;

	if (parent == nullptr) {
		std::cout << "Parent: nullptr" << std::endl;
	}
	else {
		box = parent->nodeBox;
		std::cout << "Parent: Index " << parent->boxIndex << " BOX " << box.min.x << " " << box.min.y << " " << box.min.y << " " << box.max.y << std::endl;
	}

	std::cout << std::endl;

	print(node->left);
	print(node->right);
}