#include "PhysicsSystem.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "LuaManager/LuaState.h"
#include "Physics/Rigidbody.h"

void VengefulGameEngine::PhysicsSystem::init(float initialGravityX, float initialGravityY,
	float initialStepA, int32_t initialStepB, int32_t initialStepC)
{
	gravity.x = initialGravityX;
	gravity.y = initialGravityY;
	stepA = initialStepA;
	stepB = initialStepB;
	stepC = initialStepC;
}

void VengefulGameEngine::PhysicsSystem::createWorld()
{
	world = std::make_shared<b2World>(gravity);
	world->SetContactListener(&contactListener);
}

void VengefulGameEngine::PhysicsSystem::updateAfterActors()
{
	if (world) {
		world->Step(stepA, stepB, stepC);
	}
}

VengefulGameEngine::Rigidbody* VengefulGameEngine::PhysicsSystem::createRigidbody()
{
	VengefulGameEngine::Rigidbody* rigidbody = new VengefulGameEngine::Rigidbody();
	rigidbody->rigidbodyID = rigidbodyCounter;
	rigidbodies[rigidbodyCounter] = rigidbody;
	newRigidbodyIDs.push_back(rigidbodyCounter);
	rigidbodyCounter++;
	return rigidbody;
}

VengefulGameEngine::HitResult* VengefulGameEngine::PhysicsSystem::createHitResult()
{
	HitResult* newHitResult = new HitResult();
	hitResults.push_back(newHitResult);
	return newHitResult;
}

void VengefulGameEngine::PhysicsSystem::clearHitResults()
{
	for (int i = 0; i < hitResults.size(); i++) {
		if (hitResults[i] != nullptr) {
			delete hitResults[i];
			hitResults[i] = nullptr;
		}
	}
	hitResults.clear();
}

VengefulGameEngine::PhysicsSystem::~PhysicsSystem()
{
	for (int i = 0; i < rigidbodies.size(); i++) {
		if (rigidbodies[i] != nullptr) {
			delete rigidbodies[i];
			rigidbodies[i] = nullptr;
		}
	}

	rigidbodies.clear();
	hitResults.clear();
}
