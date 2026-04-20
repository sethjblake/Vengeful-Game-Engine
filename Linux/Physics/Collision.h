#ifndef COLLISION_H
#define COLLISION_H

#include "Actors/Actor.h"
#include "box2d/box2d.h"

namespace VengefulGameEngine {
	struct Collision {
		Actor* other = nullptr;
		b2Vec2 point = { -999.0f, -999.0f };
		b2Vec2 relativeVelocity = { 0.0f, 0.0f };
		b2Vec2 normal = { -999.0f, -999.0f };
	};
}

#endif