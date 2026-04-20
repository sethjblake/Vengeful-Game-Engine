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

namespace VengefulGameEngine {

	class PhysicsSystem {
	public:

		void init(float initialGravityX, float initialGravityY,
			float initialStepA, int32_t initialStepB, int32_t initialStepC);

		void createWorld();

		void updateAfterActors();

		Rigidbody* createRigidbody();

		std::shared_ptr<b2World> world = nullptr;

		b2Vec2 gravity;
		
		float stepA;

		int32_t stepB;

		int32_t stepC;

		std::unordered_map<int, Rigidbody*> rigidbodies;

		std::vector<int> newRigidbodyIDs;

		int rigidbodyCounter = 0;

		ContactListener contactListener;

		HitResult* createHitResult();

		std::vector<HitResult*> hitResults;

		void clearHitResults();

		~PhysicsSystem();
	};
}

#endif