#ifndef AABB_TREE_STATIC_H
#define AABB_TREE_STATIC_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "AABB_Types.h"

using ID = int;

namespace CollisionSystem2D {

	struct AABB_Tree_Static_Node {
		AABB_Tree_Static_Node* left = nullptr;
		AABB_Tree_Static_Node* right = nullptr;
		AABB box;
		int leafBoxIndex = -1;
	};

	class AABB_Tree_Static {

	public:
		
		void createNewBox(ID id, float centerX, float centerY, float colliderWidth, float colliderHeight);

		void createNewBox(ID id, Point2D min, Point2D max);
		
		void constructTreeFromExisting();
		
		void getCollisionsViaID(int uniqueID, std::unordered_set<int> &collisions);
		
		void getCollisions(AABB* box, std::unordered_set<int>& collisions);

		void getCollisions(AABB* box, std::unordered_set<int>& collisions, AABB_Tree_Static_Node* node);

		void destroyNode(AABB_Tree_Static_Node* node);

		void print();

		void printBox(int uniqueID);

		void expandBox(AABB& box, float percentage);

		~AABB_Tree_Static();

	private:

		void print(AABB_Tree_Static_Node* node);

		AABB_Tree_Static_Node* createBranch(std::vector<int>::iterator begin, int size, AABB_Tree_Static_Node* parent);

		char getAxisOfSplit(std::vector<int>::iterator begin, int size, Point2D& collectiveMin, Point2D& collectiveMax);

		void partitionByAxis(std::vector<int>::iterator begin, int size, char axisOfSplit);

		Point2D getMargins(Point2D min, Point2D max, float percentage);

		AABB* getBoxByID(ID id);
		
		bool overlaps(AABB* A, AABB* B);

		AABB_Tree_Static_Node* root = nullptr;
		std::vector<AABB> leaves;
		std::vector<AABB> leavesWithMargin;
		std::unordered_map<ID, AABB_Tree_Static_Node*> uniqueIDToLeafPointerMap;
		std::unordered_map<ID, std::unordered_set<ID>> uniqueIDToCollisionSetMap;
		std::unordered_map<ID, int> uniqueIDtoLeafIndexMap;
		std::unordered_map<int, ID> leafIndexToUniqueIDMap;
	};
}

#endif