#ifndef AABB_TREE_DYNAMIC_H
#define AABB_TREE_DYNAMIC_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "AABB_Types.h"

namespace CollisionSystem2D {

	struct AABB_Tree_Dynamic_Node {
		AABB_Tree_Dynamic_Node* left = nullptr;
		AABB_Tree_Dynamic_Node* right = nullptr;
		AABB_Tree_Dynamic_Node* parent = nullptr;
		AABB nodeBox;
		int boxIndex = -1;
	};

	class AABB_Tree_Dynamic {
	public:

		void createNewBox(int id, Point2D min, Point2D max);

		void constructTreeFromExisting();

		AABB getBox(int uniqueID);

		// Do this only after checking for collisions with stationary objects
		void attemptToMove(int uniqueID, AABB* newBox, bool& didMove, std::unordered_set<int>& collisionIDs);

		void forceMove(AABB* newBox);

		void getCollisions(AABB* box, std::unordered_set<int>& collisions);

		void getCollisions(AABB* box, std::unordered_set<int>& collisions, AABB_Tree_Dynamic_Node* node);

		~AABB_Tree_Dynamic();

	private:

		void updateBox(AABB* newBox);

		void reinsertNode(AABB_Tree_Dynamic_Node* node);

		void insert(AABB_Tree_Dynamic_Node* node, AABB_Tree_Dynamic_Node* at);

		float getRegionEnlargement(AABB_Tree_Dynamic_Node* node, AABB_Tree_Dynamic_Node* other);

		void insertSibling(AABB_Tree_Dynamic_Node* adoptedChild, AABB_Tree_Dynamic_Node* biologicalChild);

		void fixRegion(AABB_Tree_Dynamic_Node* node);

		void destroyNode(AABB_Tree_Dynamic_Node* node);

		AABB_Tree_Dynamic_Node* createBranch(std::vector<int>::iterator begin, int size, AABB_Tree_Dynamic_Node* parent);

		char getAxisOfSplit(std::vector<int>::iterator begin, int size, Point2D& collectiveMin, Point2D& collectiveMax);

		void partitionByAxis(std::vector<int>::iterator begin, int size, char axisOfSplit);

		void expandBox(AABB& box, float percentage);

		bool overlaps(AABB* A, AABB* B);

		void print(AABB_Tree_Dynamic_Node* node);

		AABB_Tree_Dynamic_Node* root = nullptr;
		std::vector<AABB> boxes;
		std::vector<AABB> boxRegions;
		std::unordered_map<int, int> uniqueIDtoBoxIndexMap;
		std::unordered_map<int, int> boxIndexToUniqueIDMap;
		std::unordered_map<int, AABB_Tree_Dynamic_Node*> boxIndexToLeafNodeMap;
	};
}

#endif
