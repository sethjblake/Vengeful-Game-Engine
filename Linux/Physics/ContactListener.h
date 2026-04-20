#ifndef CONTACT_LISTENER_H
#define CONTACT_LISTENER_H

#include "box2d/box2d.h"

namespace VengefulGameEngine {
	class ContactListener : public b2ContactListener {

		void BeginContact(b2Contact* contact);

		void EndContact(b2Contact* contact);

		void OnCollisionEnter(b2Fixture* fixture, b2Fixture* other,
			b2Vec2 point, b2Vec2 relativeVelocity, b2Vec2 normal);

		void OnTriggerEnter(b2Fixture* fixture, b2Fixture* other, b2Vec2 relativeVelocity);

		void OnTriggerExit(b2Fixture* fixture, b2Fixture* other, b2Vec2 relativeVelocity);

		void OnCollisionExit(b2Fixture* fixture, b2Fixture* other, b2Vec2 relativeVelocity);
	};
}

#endif