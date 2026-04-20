#ifndef RAYCAST_H
#define RAYCAST_H

#include "box2d/box2d.h"
#include "Physics/HitResult.h"
#include "Physics/GlobalPhysicsSystem.h"
#include <stdfloat>
#include "../../ThirdParty/glm/glm/glm.hpp"
#include "../../ThirdParty/Lua/lua.hpp"
#include "../../ThirdParty/LuaBridge/LuaBridge.h"
#include "LuaManager/LuaState.h"
#include <utility>
#include <map>

namespace VengefulGameEngine {

	class SingleRayCastCallback : public b2RayCastCallback {
	public:
		HitResult* hitResult;
		float minFraction = 1.0f;

		SingleRayCastCallback() {
			hitResult = globalPhysicsSystem.createHitResult();
		}

		bool noCollisions() {
			return hitResult->actor == nullptr;
		}

		float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
			const b2Vec2& normal, float fraction) {

			if (fraction == 0.0f) return 1.0f;

			if (fixture->GetFilterData().categoryBits == 0) {
				return 1.0f;
			}

			if (hitResult->actor == nullptr || fraction < minFraction) {
				minFraction = fraction;
				b2FixtureUserData data = fixture->GetUserData();
				hitResult->actor = reinterpret_cast<Actor*>(data.pointer);
				hitResult->normal = normal;
				hitResult->point = point;
				hitResult->isTrigger = fixture->IsSensor();
			}

			return fraction;
		}
	};

	class MultiRayCastCallback : public b2RayCastCallback {
	public:
		std::map<float, HitResult*> hitResults;

		bool noCollisions() {
			return hitResults.empty();
		}

		float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
			const b2Vec2& normal, float fraction) {

			if (fraction == 0.0f) return 1.0f;

			if (fixture->GetFilterData().categoryBits == 0) {
				return 1.0f;
			}

			b2FixtureUserData data = fixture->GetUserData();
			Actor* actor = reinterpret_cast<Actor*>(data.pointer);
			if (actor == nullptr) return -1;

			HitResult* hitResult = globalPhysicsSystem.createHitResult();
			hitResult->actor = actor;
			hitResult->normal = normal;
			hitResult->point = point;
			hitResult->isTrigger = fixture->IsSensor();

			hitResults[fraction] = hitResult;

			return 1.0f;
		}
	};

	luabridge::LuaRef RayCast(b2Vec2 pos, b2Vec2 dir, float dist) {

		b2Vec2 endpoint = pos + (dist * dir);

		SingleRayCastCallback callback;

		globalPhysicsSystem.world->RayCast(&callback, pos, endpoint);

		if (callback.noCollisions()) {
			return luabridge::LuaRef(luaState);
		}

		return luabridge::LuaRef(luaState, *callback.hitResult);
	}

	luabridge::LuaRef RayCastAll(b2Vec2 pos, b2Vec2 dir, float dist) {

		b2Vec2 endpoint = pos + (dist * dir);

		MultiRayCastCallback callback;

		globalPhysicsSystem.world->RayCast(&callback, pos, endpoint);

		luabridge::LuaRef newTable = luabridge::newTable(luaState);

		if (callback.noCollisions()) {
			return newTable;
		}

		int luaIndex = 1;
		for (auto& item : callback.hitResults) {
			newTable[luaIndex++] = *item.second;
		}

		return newTable;
	}
}
#endif