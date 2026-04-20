#include "Physics/ContactListener.h"
#include <iostream>
#include "Actors/Actor.h"
#include "Physics/Collision.h"
#include "Physics/GlobalPhysicsSystem.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include <algorithm>

void VengefulGameEngine::ContactListener::BeginContact(b2Contact* contact) {

	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);
	b2Vec2 point = worldManifold.points[0];
	b2Vec2 normal = worldManifold.normal;
	b2Vec2 relativeVelocity = contact->GetFixtureA()->GetBody()->GetLinearVelocity() - contact->GetFixtureB()->GetBody()->GetLinearVelocity();

	if (!contact->GetFixtureA()->IsSensor()) {
		OnCollisionEnter(contact->GetFixtureA(), contact->GetFixtureB(), point, relativeVelocity, normal);
		OnCollisionEnter(contact->GetFixtureB(), contact->GetFixtureA(), point, relativeVelocity, normal);
	}
	else {
		OnTriggerEnter(contact->GetFixtureA(), contact->GetFixtureB(), relativeVelocity);
		OnTriggerEnter(contact->GetFixtureB(), contact->GetFixtureA(), relativeVelocity);
	}
}

void VengefulGameEngine::ContactListener::EndContact(b2Contact* contact) {

	b2Vec2 relativeVelocity = contact->GetFixtureA()->GetBody()->GetLinearVelocity() - contact->GetFixtureB()->GetBody()->GetLinearVelocity();

	if (!contact->GetFixtureA()->IsSensor()) {
		OnCollisionExit(contact->GetFixtureA(), contact->GetFixtureB(), relativeVelocity);
		OnCollisionExit(contact->GetFixtureB(), contact->GetFixtureA(), relativeVelocity);
	}
	else {
		OnTriggerExit(contact->GetFixtureA(), contact->GetFixtureB(), relativeVelocity);
		OnTriggerExit(contact->GetFixtureB(), contact->GetFixtureA(), relativeVelocity);
	}
}

void VengefulGameEngine::ContactListener::OnCollisionEnter(b2Fixture* fixture, b2Fixture* other, b2Vec2 point, b2Vec2 relativeVelocity, b2Vec2 normal)
{
	b2FixtureUserData userData = fixture->GetUserData();
	uintptr_t pointer = userData.pointer;
	Actor* actor = reinterpret_cast<Actor*>(pointer);

	b2FixtureUserData userDataOther = other->GetUserData();
	uintptr_t otherPointer = userDataOther.pointer;
	Actor* otherActor = reinterpret_cast<Actor*>(otherPointer);

	Collision collision;
	collision.other = otherActor;
	collision.point = point;
	collision.relativeVelocity = relativeVelocity;
	collision.normal = normal;

	for (auto& item : actor->components) {
		std::shared_ptr<Component> component = item.second;
		std::shared_ptr<luabridge::LuaRef> componentRef = component->luaRef;
		luabridge::LuaRef onCollisionEnterFunction = (*componentRef)["OnCollisionEnter"];
		if (!onCollisionEnterFunction.isNil()) {
			try {
				(*componentRef)["OnCollisionEnter"](*componentRef, collision);
			}
			catch (const luabridge::LuaException& e) {
				std::string errorMessage = e.what();

				std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');

				std::cout << "\033[31m" << actor->name << " : " << errorMessage << "\033[0m" << std::endl;
			}
			
		}
	}
}

void VengefulGameEngine::ContactListener::OnTriggerEnter(b2Fixture* fixture, b2Fixture* other, b2Vec2 relativeVelocity)
{
	b2FixtureUserData userData = fixture->GetUserData();
	uintptr_t pointer = userData.pointer;
	Actor* actor = reinterpret_cast<Actor*>(pointer);

	b2FixtureUserData userDataOther = other->GetUserData();
	uintptr_t otherPointer = userDataOther.pointer;
	Actor* otherActor = reinterpret_cast<Actor*>(otherPointer);

	Collision collision;
	collision.other = otherActor;
	collision.relativeVelocity = relativeVelocity;

	for (auto& item : actor->components) {
		std::shared_ptr<Component> component = item.second;
		std::shared_ptr<luabridge::LuaRef> componentRef = component->luaRef;
		luabridge::LuaRef onCollisionEnterFunction = (*componentRef)["OnTriggerEnter"];
		if (!onCollisionEnterFunction.isNil()) {
			try {
				(*componentRef)["OnTriggerEnter"](*componentRef, collision);
			}
			catch (const luabridge::LuaException& e) {
				std::string errorMessage = e.what();

				std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');

				std::cout << "\033[31m" << actor->name << " : " << errorMessage << "\033[0m" << std::endl;
			}

		}
	}
}

void VengefulGameEngine::ContactListener::OnCollisionExit(b2Fixture* fixture, b2Fixture* other, b2Vec2 relativeVelocity)
{
	b2FixtureUserData userData = fixture->GetUserData();
	uintptr_t pointer = userData.pointer;
	Actor* actor = reinterpret_cast<Actor*>(pointer);

	b2FixtureUserData userDataOther = other->GetUserData();
	uintptr_t otherPointer = userDataOther.pointer;
	Actor* otherActor = reinterpret_cast<Actor*>(otherPointer);

	Collision collision;
	collision.other = otherActor;
	collision.relativeVelocity = relativeVelocity;

	for (auto& item : actor->components) {
		std::shared_ptr<Component> component = item.second;
		std::shared_ptr<luabridge::LuaRef> componentRef = component->luaRef;
		luabridge::LuaRef onCollisionEnterFunction = (*componentRef)["OnCollisionExit"];
		if (!onCollisionEnterFunction.isNil()) {
			try {
				(*componentRef)["OnCollisionExit"](*componentRef, collision);
			}
			catch (const luabridge::LuaException& e) {
				std::string errorMessage = e.what();

				std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');

				std::cout << "\033[31m" << actor->name << " : " << errorMessage << "\033[0m" << std::endl;
			}

		}
	}
}

void VengefulGameEngine::ContactListener::OnTriggerExit(b2Fixture* fixture, b2Fixture* other, b2Vec2 relativeVelocity)
{
	b2FixtureUserData userData = fixture->GetUserData();
	uintptr_t pointer = userData.pointer;
	Actor* actor = reinterpret_cast<Actor*>(pointer);

	b2FixtureUserData userDataOther = other->GetUserData();
	uintptr_t otherPointer = userDataOther.pointer;
	Actor* otherActor = reinterpret_cast<Actor*>(otherPointer);

	Collision collision;
	collision.other = otherActor;
	collision.relativeVelocity = relativeVelocity;

	for (auto& item : actor->components) {
		std::shared_ptr<Component> component = item.second;
		std::shared_ptr<luabridge::LuaRef> componentRef = component->luaRef;
		luabridge::LuaRef onCollisionEnterFunction = (*componentRef)["OnTriggerExit"];
		if (!onCollisionEnterFunction.isNil()) {
			try {
				(*componentRef)["OnTriggerExit"](*componentRef, collision);
			}
			catch (const luabridge::LuaException& e) {
				std::string errorMessage = e.what();

				std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');

				std::cout << "\033[31m" << actor->name << " : " << errorMessage << "\033[0m" << std::endl;
			}

		}
	}
}
