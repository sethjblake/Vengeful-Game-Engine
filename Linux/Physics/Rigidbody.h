#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <string>
#include <memory>
#include "box2d/box2d.h"
#include "GlobalRigidbodyConstants.h"
#include "Actors/Actor.h"

namespace VengefulGameEngine {

	class Rigidbody {
	public:

		Actor* actor = nullptr;

		std::string key;

		bool enabled = true;

		Rigidbody();

		void OnStart();

		int parentActorID = -1;

		float x = 0.0f;

		float y = 0.0f;

		// Collider

		std::string bodyType = globalRigidbodyTypes.DYNAMIC;

		b2BodyType bodyTypeBox2D = b2_dynamicBody;

		bool precise = true;

		float gravityScale = 1.0f;

		float density = 1.0f;

		float angularFriction = 0.3f;

		float rotation = 0.0f;

		bool hasCollider = true;

		bool hasTrigger = true;

		b2BodyDef colliderBodyDefBox2D;

		b2Body* body;

		int rigidbodyID = -1;

		std::string colliderType = globalRigidbodyColliderTypes.BOX;

		float colliderWidth = 1.0f;

		float colliderHeight = 1.0f;

		float colliderRadius = 0.5f;

		float friction = 0.3f;

		float colliderBounciness = 0.3f;

		b2Vec2 startingVelocity = { 0.0f, 0.0f };

		b2Vec2 startingForce = { 0.0f, 0.0f };

		float startingAngularVelocity = 0.0f;

		// Trigger

		std::string triggerType = globalRigidbodyColliderTypes.BOX;

		float triggerWidth = 1.0f;

		float triggerHeight = 1.0f;

		float triggerRadius = 0.5f;

		b2Body* triggerBody;

		b2Vec2 getPosition();

		float getRotation();

		void setBodyType(std::string type);

		void addForce(b2Vec2 forceToAdd);

		void setVelocity(b2Vec2 newVelocity);

		void setPosition(b2Vec2 newPosition);

		void setRotation(float degreesClockwise);

		void setAngularVelocity(float degreesClockwise);

		void setGravityScale(float newGravityScale);

		void setUpDirection(b2Vec2 newUpDirection);

		void setRightDirection(b2Vec2 newRightDirection);

		b2Vec2 getVelocity();

		float getAngularVelocity();

		float getGravityScale();

		b2Vec2 getUpDirection();

		b2Vec2 getRightDirection();

		void onDestroy();

	};
}

#endif