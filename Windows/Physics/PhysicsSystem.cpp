#include "PhysicsSystem.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "LuaManager/LuaState.h"
#include "Physics/Rigidbody.h"
#include "Math/Conversions.h";
#include "Actors/GlobalActorsMap.h"
#include "Physics/Body.h"
#include "Physics/Collider.h"
#include "Physics/Trigger.h"
#include <utility>

void VengefulGameEngine::PhysicsSystem::init(float initialGravityX, float initialGravityY,
	float initialStepA, int32_t initialStepB, int32_t initialStepC)
{
	gravity.x = initialGravityX;
	gravity.y = initialGravityY;
	stepA = initialStepA;
	stepB = initialStepB;
	stepC = initialStepC;
}

bool VengefulGameEngine::PhysicsSystem::worldExists()
{
	return world != nullptr;
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

VengefulGameEngine::Rigidbody* VengefulGameEngine::PhysicsSystem::createRigidbody(int uniqueComponentID)
{
	VengefulGameEngine::Rigidbody* rigidbody = new VengefulGameEngine::Rigidbody();
	rigidbody->rigidbodyID = rigidbodyCounter;
	rigidbodies[uniqueComponentID] = rigidbody;
	newRigidbodyIDs.push_back(uniqueComponentID);
	return rigidbody;
}

void VengefulGameEngine::PhysicsSystem::initRigidbodiesOnUpdate()
{
	if (newRigidbodyIDs.empty()) return;

	for (int id : newRigidbodyIDs) {
		initRigidbody(id);
	}
	newRigidbodyIDs.clear();
}

void VengefulGameEngine::PhysicsSystem::destroyRigidbody(int uniqueComponentID)
{
	Rigidbody* rigidbody = rigidbodies[uniqueComponentID];
	rigidbodies.erase(uniqueComponentID);
	world->DestroyBody(rigidbody->body.b2BodyPtr);
	delete rigidbody;
}

void VengefulGameEngine::PhysicsSystem::initRigidbody(int uniqueComponentID)
{
	Rigidbody* rigidbody = rigidbodies[uniqueComponentID];

	rigidbody->setBodyType(rigidbody->getBodyType());
	Body& body = rigidbody->body;
	body.bodyDef.position = rigidbody->position;
	body.bodyDef.type = body.bodyTypeBox2D;
	body.bodyDef.bullet = body.precise;
	body.bodyDef.gravityScale = body.angularFriction;
	body.bodyDef.angle = degreesToRadians(body.rotation);

	body.b2BodyPtr = world->CreateBody(&body.bodyDef);

	body.b2BodyPtr->SetLinearVelocity(body.startingVelocity);
	body.b2BodyPtr->ApplyForceToCenter(body.startingForce, true);
	body.b2BodyPtr->SetAngularVelocity(degreesToRadians(body.startingAngularVelocity));

	if (body.hasCollider) {
		b2FixtureDef fixtureDef;
		b2PolygonShape boxShape;
		b2CircleShape circleShape;

		Collider& collider = rigidbody->collider;

		if (collider.shape == globalRigidbodyColliderTypes.BOX) {
			boxShape.SetAsBox(rigidbody->collider.width * 0.5f, rigidbody->collider.height * 0.5f);
			fixtureDef.shape = &boxShape;
		}
		else if (collider.shape == globalRigidbodyColliderTypes.CIRCLE) {
			circleShape.m_radius = rigidbody->collider.radius;
			fixtureDef.shape = &circleShape;
		}
		
		fixtureDef.density = body.density;
		fixtureDef.restitution = body.bounciness;
		fixtureDef.friction = body.friction;
		fixtureDef.isSensor = false;
		Actor* actor = globalActorsMap[rigidbody->parentActorID];
		fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(actor);
		fixtureDef.filter.categoryBits = 0b1;
		fixtureDef.filter.maskBits = 0b1;
		b2Fixture* fixture = body.b2BodyPtr->CreateFixture(&fixtureDef);
	}

	if (body.hasTrigger) {
		b2FixtureDef fixtureDef;
		b2PolygonShape boxShape;
		b2CircleShape circleShape;

		Trigger& trigger = rigidbody->trigger;

		if (trigger.shape == globalRigidbodyColliderTypes.BOX) {
			boxShape.SetAsBox(trigger.width * 0.5f, trigger.height * 0.5f);
			fixtureDef.shape = &boxShape;
		}
		else if (trigger.shape == globalRigidbodyColliderTypes.CIRCLE) {
			circleShape.m_radius = trigger.radius;
			fixtureDef.shape = &circleShape;
		}
		fixtureDef.isSensor = true;
		fixtureDef.density = body.density;
		Actor* actor = globalActorsMap[rigidbody->parentActorID];
		fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(actor);
		fixtureDef.filter.categoryBits = 0b10;
		fixtureDef.filter.maskBits = 0b10;
		body.b2BodyPtr->CreateFixture(&fixtureDef);
	}

	if (!body.hasCollider && !body.hasTrigger) {

		Collider& collider = rigidbody->collider;

		b2PolygonShape phantomShape;
		phantomShape.SetAsBox(collider.width * 0.5f, collider.height * 0.5f);

		b2FixtureDef phantomFixtureDef;
		phantomFixtureDef.shape = &phantomShape;
		phantomFixtureDef.density = body.density;

		phantomFixtureDef.isSensor = true;
		phantomFixtureDef.filter.categoryBits = 0;
		phantomFixtureDef.filter.maskBits = 0;
		Actor* actor = globalActorsMap[rigidbody->parentActorID];
		phantomFixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(actor);
		body.b2BodyPtr->CreateFixture(&phantomFixtureDef);
	}
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
