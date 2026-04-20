#include "AABB_Tree_Static.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include "AABB_Tree_Dynamic.h"

void CollisionSystem2D::AABB_Tree_Static::createNewBox(ID id, float centerX, float centerY, float width, float height)
{
	Point2D min, max;

	float halfWidth = width / 2;
	min.x = centerX - halfWidth;
	max.x = centerX + halfWidth;

	float halfHeight = height / 2;
	min.y = centerY - halfHeight;
	max.y = centerY + halfHeight;

	createNewBox(id, min, max);
}

void CollisionSystem2D::AABB_Tree_Static::createNewBox(ID id, Point2D min, Point2D max)
{
	AABB newBox;
	newBox.uniqueID = id;
	newBox.min = min;
	newBox.max = max;

	AABB newBoxWithMargins = newBox;
	expandBox(newBoxWithMargins, 0.0f);

	uniqueIDtoLeafIndexMap[id] = leaves.size();
	leafIndexToUniqueIDMap[leaves.size()] = id;

	leaves.push_back(newBox);
	leavesWithMargin.push_back(newBoxWithMargins);
}

void CollisionSystem2D::AABB_Tree_Static::constructTreeFromExisting()
{
	std::vector<int> ids;
	for (int i = 0; i < leaves.size(); i++) { ids.push_back(i); }

	root = createBranch(ids.begin(), ids.size(), root);
}

CollisionSystem2D::AABB_Tree_Static_Node* CollisionSystem2D::AABB_Tree_Static::createBranch(std::vector<int>::iterator begin,
																				int size, AABB_Tree_Static_Node* parent)
{
	if (size == 0) return nullptr;

	if (size == 1) {
		int leafIndex = *begin;
		AABB& box = leavesWithMargin[leafIndex];

		AABB_Tree_Static_Node* leafNode = new AABB_Tree_Static_Node();
		
		leafNode->box.uniqueID = leafIndexToUniqueIDMap[leafIndex];
		
		uniqueIDToLeafPointerMap[leafNode->box.uniqueID] = leafNode;
		
		leafNode->box.min = box.min;
		leafNode->box.max = box.max;

		leafNode->left = nullptr;
		leafNode->right = nullptr;

		leafNode->leafBoxIndex = leafIndex;

		return leafNode;
	}
	
	Point2D collectiveMin, collectiveMax;

	char axisOfSplit = getAxisOfSplit(begin, size, collectiveMin, collectiveMax);

	partitionByAxis(begin, size, axisOfSplit);

	int leftBranchSize{ size / 2 };
	int rightBranchSize{ size - leftBranchSize };

	AABB_Tree_Static_Node* splitNode = new AABB_Tree_Static_Node();

	splitNode->left = createBranch(begin, leftBranchSize, splitNode);
	splitNode->right = createBranch(begin + leftBranchSize, rightBranchSize, splitNode);

	splitNode->box.min.x = collectiveMin.x;
	splitNode->box.min.y = collectiveMin.y;
	splitNode->box.max.x = collectiveMax.x;
	splitNode->box.max.y = collectiveMax.y;
	splitNode->box.uniqueID = -1;

	return splitNode;
}

char CollisionSystem2D::AABB_Tree_Static::getAxisOfSplit(std::vector<int>::iterator begin, int size, Point2D& collectiveMin, Point2D& collectiveMax)
{
	int first = *begin;

	float minX = leavesWithMargin[first].min.x;

	for (std::vector<int>::iterator itr = begin; itr < begin + size; itr++) {
		AABB& leaf = leavesWithMargin[*itr];
		if (leaf.min.x < minX) minX = leaf.min.x;
	}

	float maxX = leavesWithMargin[first].max.x;

	for (std::vector<int>::iterator itr = begin; itr < begin + size; itr++) {
		AABB& leaf = leavesWithMargin[*itr];
		if (leaf.max.x > maxX) maxX = leaf.max.x;
	}

	float distanceX = maxX - minX;

	collectiveMin.x = minX;
	collectiveMax.x = maxX;

	float minY = leavesWithMargin[first].min.y;

	for (std::vector<int>::iterator itr = begin; itr < begin + size; itr++) {
		AABB& leaf = leavesWithMargin[*itr];
		if (leaf.min.y < minY) minY = leaf.min.y;
	}

	float maxY = leavesWithMargin[first].max.y;

	for (std::vector<int>::iterator itr = begin; itr < begin + size; itr++) {
		AABB& leaf = leavesWithMargin[*itr];
		if (leaf.max.y > maxY) maxY = leaf.max.y;
	}

	float distanceY = maxY - minY;

	collectiveMin.y = minY;
	collectiveMax.y = maxY;

	if (distanceY > distanceX) {
		return 'Y';
	}

	return 'X';
}

void CollisionSystem2D::AABB_Tree_Static::partitionByAxis(std::vector<int>::iterator begin, int size, char axisOfSplit)
{

	if (axisOfSplit == 'Y') {

		std::nth_element(begin, begin + (size / 2), begin + size, [this](const int& A, const int& B) {
			float centerA;
			centerA = this->leavesWithMargin[A].min.y + ((this->leavesWithMargin[A].max.y - this->leavesWithMargin[A].min.y) / 2);

			float centerB;
			centerB = this->leavesWithMargin[B].min.y + ((this->leavesWithMargin[B].max.y - this->leavesWithMargin[B].min.y) / 2);

			return centerA < centerB;
			});

		return;
	}

	std::nth_element(begin, begin + (size / 2), begin + size, [this](const int& A, const int& B) {
		float centerA;
		centerA = this->leavesWithMargin[A].min.x + ((this->leavesWithMargin[A].max.x - this->leavesWithMargin[A].min.x) / 2);

		float centerB;
		centerB = this->leavesWithMargin[B].min.x + ((this->leavesWithMargin[B].max.x - this->leavesWithMargin[B].min.x) / 2);

		return centerA < centerB;
		});
}

CollisionSystem2D::Point2D CollisionSystem2D::AABB_Tree_Static::getMargins(Point2D min, Point2D max, float percentage)
{
	Point2D margins;
	margins.x = (max.x - min.x) * percentage;
	margins.y = (max.y - min.y) * percentage;
	return margins;
}

void CollisionSystem2D::AABB_Tree_Static::expandBox(AABB& box, float percentage)
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

void CollisionSystem2D::AABB_Tree_Static::getCollisions(AABB* box, std::unordered_set<int>& collisions, AABB_Tree_Static_Node* node)
{
	if (node == nullptr) {
		return;
	}

	if (node->left != nullptr) {
		if (overlaps(&node->left->box, box)) {
			getCollisions(box, collisions, node->left);
		}
	}
	if (node->right != nullptr) {
		if (overlaps(&node->right->box, box)) {
			getCollisions(box, collisions, node->right);
		}
	}

	if (node->right == nullptr && node->left == nullptr) {

		int leafIndex = uniqueIDtoLeafIndexMap[node->box.uniqueID];
		AABB& leaf = leaves[leafIndex];

		if (overlaps(&leaf, box)) {
			collisions.insert(node->box.uniqueID);

			//std::cout << "Collision Detected:\n";

			//AABB box1 = node->box;

			//std::cout << node->leafBoxIndex << " ";
			//std::cout << "BOX " << box1.uniqueID;
			//std::cout << " MIN " << box1.min.x << " " << box1.min.y;
			//std::cout << " MAX " << box1.max.x << " " << box1.max.y;
			//std::cout << std::endl;

			//box1 = *box;

			//std::cout << node->leafBoxIndex << " ";
			//std::cout << "BOX " << box1.uniqueID;
			//std::cout << " MIN " << box1.min.x << " " << box1.min.y;
			//std::cout << " MAX " << box1.max.x << " " << box1.max.y;
			//std::cout << std::endl;

			//for (int i = 0; i < leaves.size(); i++) {
			//	box1 = leaves[i];

			//	std::cout << node->leafBoxIndex << " ";
			//	std::cout << "BOX " << box1.uniqueID;
			//	std::cout << " MIN " << box1.min.x << " " << box1.min.y;
			//	std::cout << " MAX " << box1.max.x << " " << box1.max.y;
			//	std::cout << std::endl;
			//}

		}
	}
}

bool CollisionSystem2D::AABB_Tree_Static::overlaps(AABB* A, AABB* B)
{
	bool noCollisionX = A->max.x < B->min.x || B->max.x < A->min.x;

	if (noCollisionX) return false;

	bool noCollisionY = A->max.y < B->min.y || B->max.y < A->min.y;

	if (noCollisionY) return false;

	return true;

}


void CollisionSystem2D::AABB_Tree_Static::getCollisionsViaID(int uniqueID, std::unordered_set<int>& collisions)
{
	collisions = uniqueIDToCollisionSetMap[uniqueID];
}

void CollisionSystem2D::AABB_Tree_Static::getCollisions(AABB* box, std::unordered_set<int>& collisions)
{
	getCollisions(box, collisions, root);
}

CollisionSystem2D::AABB* CollisionSystem2D::AABB_Tree_Static::getBoxByID(ID id)
{
	return &leaves[uniqueIDtoLeafIndexMap[id]];
}

void CollisionSystem2D::AABB_Tree_Static::destroyNode(AABB_Tree_Static_Node* node)
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

CollisionSystem2D::AABB_Tree_Static::~AABB_Tree_Static()
{
	destroyNode(root);
}

void CollisionSystem2D::AABB_Tree_Static::print() {

	std::vector<int> ids;

	for (auto item : leafIndexToUniqueIDMap) {
		ids.push_back(item.second);
	}

	std::sort(ids.begin(), ids.end());

	for (int id : ids) {
		int leafIndex = uniqueIDtoLeafIndexMap[id];
		AABB& box = leaves[leafIndex];
		std::cout << id << " ";
		std::cout << "BOX ";
		std::cout << box.min.x << " " << box.min.y << " ";
		std::cout << box.max.x << " " << box.max.y << " ";
		std::cout << std::endl;
	}

	//if (root != nullptr) {
	//	print(root);
	//}
}

void CollisionSystem2D::AABB_Tree_Static::printBox(int uniqueID)
{
	int leafIndex = uniqueIDtoLeafIndexMap[uniqueID];
	AABB& box = leaves[leafIndex];
	std::cout << uniqueID << " ";
	std::cout << "BOX ";
	std::cout << box.min.x << " " << box.min.y << " ";
	std::cout << box.max.x << " " << box.max.y << " ";
	std::cout << std::endl;
}

void CollisionSystem2D::AABB_Tree_Static::print(AABB_Tree_Static_Node* node) {

	if (node == nullptr) {
		return;
	}

	if (node->box.uniqueID != -1) {

		AABB& box = leaves[node->leafBoxIndex];

		std::cout << node->leafBoxIndex << " ";
		std::cout << "BOX " << box.uniqueID;
		std::cout << " MIN " << box.min.x << " " << box.min.y;
		std::cout << " MAX " << box.max.x << " " << box.max.y;
		std::cout << std::endl;
	}
	else {
		AABB& box = node->box;
		std::cout << node->leafBoxIndex << " ";
		std::cout << "BOX " << box.uniqueID;
		std::cout << " MIN " << box.min.x << " " << box.min.y;
		std::cout << " MAX " << box.max.x << " " << box.max.y;
		std::cout << std::endl;
	}

	if (node->left != nullptr) {
		print(node->left);
	}

	if (node->right != nullptr) {
		print(node->right);
	}

}