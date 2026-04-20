#ifndef HIT_RESULT_H
#define HIT_RESULT_H

#include "Actors/Actor.h"
#include "box2d/box2d.h"

namespace VengefulGameEngine {
	struct HitResult {
		Actor* actor = nullptr;
		b2Vec2 point = { 0.0f, 0.0f };
		b2Vec2 normal = { 0.0f, 0.0f };
		bool isTrigger = false;
	};
}

#endif