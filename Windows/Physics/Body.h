#ifndef BODY_H
#define BODY_H

#include <string>
#include "box2d/box2d.h"
#include "Physics/GlobalRigidbodyConstants.h"

namespace VengefulGameEngine {
	struct Body {

		b2Body* b2BodyPtr;

		std::string bodyTypeString = globalRigidbodyTypes.DYNAMIC;

		b2BodyType bodyTypeBox2D = b2_dynamicBody;

		bool precise = true;

		float startingGravityScale = 1.0f;

		float density = 1.0f;

		float angularFriction = 0.3f;

		float rotation = 0.0f;

		bool hasCollider = true;

		bool hasTrigger = true;

		b2Vec2 startingForce = { 0.0f, 0.0f };

		b2Vec2 startingVelocity = { 0.0f, 0.0f };

		float startingAngularVelocity = 0.0f;

		float friction = 0.3f;

		float bounciness = 0.3f;

		b2BodyDef bodyDef;
	};
}

#endif