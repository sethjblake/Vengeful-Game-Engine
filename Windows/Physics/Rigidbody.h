#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <string>
#include <memory>
#include "box2d/box2d.h"
#include "GlobalRigidbodyConstants.h"
#include "Actors/Actor.h"
#include "Physics/Body.h"
#include "Physics/Collider.h"
#include "Physics/Trigger.h"

namespace VengefulGameEngine {

	class Rigidbody {
	public:

		Actor* actor = nullptr;

		std::string key;

		bool enabled = true;

		int rigidbodyID = -1;

		int parentActorID = -1;

		b2Vec2 position;

		Body body;

		Collider collider;
		
		Trigger trigger;

		b2Vec2 getPosition() const;

		void setPosition(b2Vec2 newPosition);

		float getX() const;

		void setX(float x);

		float getY() const;

		void setY(float y);

		float getAngularFriction() const;

		void setAngularFruction(float f);

		float getDensity() const;

		void setDensity(float d);

		bool getHasCollider() const;

		void setHasCollider(bool h);

		bool getHasTrigger() const;

		void setHasTrigger(bool h);

		bool getPrecise() const;

		void setPrecise(bool p);

		std::string getColliderType() const;

		void setColliderType(std::string t);

		float getColliderWidth() const;

		void setColliderWidth(float w);

		float getColliderRadius() const;

		void setColliderRadius(float r);

		float getColliderHeight() const;

		void setColliderHeight(float h);

		std::string getTriggerType() const;

		void setTriggerType(std::string t);

		float getTriggerWidth() const;

		void setTriggerWidth(float w);

		float getTriggerRadius() const;

		void setTriggerRadius(float r);

		float getTriggerHeight() const;

		void setTriggerHeight(float h);

		float getBounciness() const;

		void setBounciness(float b);

		float getFriction() const;

		void setFriction(float f);

		float getRotation() const;

		void setRotation(float degreesClockwise);

		std::string getBodyType() const;

		void setBodyType(std::string type);

		b2Vec2 getVelocity() const;

		void setVelocity(b2Vec2 newVelocity);

		float getAngularVelocity() const;

		void setAngularVelocity(float degreesClockwise);

		float getGravityScale() const;

		void setGravityScale(float newGravityScale);

		void addForce(b2Vec2 forceToAdd);

		b2Vec2 getUpDirection() const;

		void setUpDirection(b2Vec2 newUpDirection);

		b2Vec2 getRightDirection() const;

		void setRightDirection(b2Vec2 newRightDirection);

	};
}

#endif