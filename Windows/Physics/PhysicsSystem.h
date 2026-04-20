#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "box2d/box2d.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include "Physics/Rigidbody.h"
#include "Physics/ContactListener.h"
#include "Physics/Collision.h"
#include "Physics/HitResult.h"
#include "unordered_set"

namespace VengefulGameEngine {

	class PhysicsSystem {
	public:

		void init(float initialGravityX, float initialGravityY,
			float initialStepA, int32_t initialStepB, int32_t initialStepC);

		bool worldExists();

		void createWorld();

		void updateAfterActors();

		Rigidbody* createRigidbody(int uniqueComponentID);

		void initRigidbodiesOnUpdate();

		void destroyRigidbody(int uniqueComponentID);

		void initRigidbody(int uniqueComponentID);

		std::shared_ptr<b2World> world = nullptr;

		b2Vec2 gravity;
		
		float stepA;

		int32_t stepB;

		int32_t stepC;

		std::unordered_map<int, Rigidbody*> rigidbodies;

		std::vector<int> newRigidbodyIDs;

		std::unordered_set<int> destroyRigidbodyIDs;

		int rigidbodyCounter = 0;

		ContactListener contactListener;

		HitResult* createHitResult();

		std::vector<HitResult*> hitResults;

		void clearHitResults();

		~PhysicsSystem();
	};
}

#endif