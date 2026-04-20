#include "Rigidbody.h"
#include "GlobalPhysicsSystem.h"
#include "Math/Conversions.h"
#include <iostream>
#include <cstdlib>
#include "glm/glm/glm.hpp"
#include "Actors/GlobalActorsMap.h"

VengefulGameEngine::Rigidbody::Rigidbody()
{
	if (globalPhysicsSystem.world == nullptr) {
		globalPhysicsSystem.createWorld();
	}

	body = nullptr;
}

void VengefulGameEngine::Rigidbody::OnStart()
{
	setBodyType(bodyType);
	colliderBodyDefBox2D.position.x = x;
	colliderBodyDefBox2D.position.y = y;
	colliderBodyDefBox2D.type = bodyTypeBox2D;
	colliderBodyDefBox2D.bullet = precise;
	colliderBodyDefBox2D.gravityScale = gravityScale;
	colliderBodyDefBox2D.angularDamping = angularFriction;
	colliderBodyDefBox2D.angle = degreesToRadians(rotation);
	
	body = globalPhysicsSystem.world->CreateBody(&colliderBodyDefBox2D);

	body->SetLinearVelocity(startingVelocity);
	body->ApplyForceToCenter(startingForce, true);
	body->SetAngularVelocity(degreesToRadians(startingAngularVelocity));

	if (hasCollider) {
		b2FixtureDef fixtureDef;
		b2PolygonShape boxShape;
		b2CircleShape circleShape;

		if (colliderType == globalRigidbodyColliderTypes.BOX) {
			boxShape.SetAsBox(colliderWidth * 0.5f, colliderHeight * 0.5f);
			fixtureDef.shape = &boxShape;
		}
		else if (colliderType == globalRigidbodyColliderTypes.CIRCLE) {
			circleShape.m_radius = colliderRadius;
			fixtureDef.shape = &circleShape;
		}

		fixtureDef.density = density;
		fixtureDef.restitution = colliderBounciness;
		fixtureDef.friction = friction;
		fixtureDef.isSensor = false;
		Actor* actor = globalActorsMap[parentActorID];
		fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(actor);
		fixtureDef.filter.categoryBits = 0b1;
		fixtureDef.filter.maskBits = 0b1;
		b2Fixture* fixture = body->CreateFixture(&fixtureDef);
	}

	if (hasTrigger) {
		b2FixtureDef fixtureDef;
		b2PolygonShape boxShape;
		b2CircleShape circleShape;

		if (triggerType == globalRigidbodyColliderTypes.BOX) {
			boxShape.SetAsBox(triggerWidth * 0.5f, triggerHeight * 0.5f);
			fixtureDef.shape = &boxShape;
		}
		else if (triggerType == globalRigidbodyColliderTypes.CIRCLE) {
			circleShape.m_radius = triggerRadius;
			fixtureDef.shape = &circleShape;
		}
		fixtureDef.isSensor = true;
		fixtureDef.density = density;
		Actor* actor = globalActorsMap[parentActorID];
		fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(actor);
		fixtureDef.filter.categoryBits = 0b10;
		fixtureDef.filter.maskBits = 0b10;
		body->CreateFixture(&fixtureDef);
	}

	if (!hasCollider && !hasTrigger) {
		b2PolygonShape phantomShape;
		phantomShape.SetAsBox(colliderWidth * 0.5f, colliderHeight * 0.5f);

		b2FixtureDef phantomFixtureDef;
		phantomFixtureDef.shape = &phantomShape;
		phantomFixtureDef.density = density;

		phantomFixtureDef.isSensor = true;
		phantomFixtureDef.filter.categoryBits = 0;
		phantomFixtureDef.filter.maskBits = 0;
		Actor* actor = globalActorsMap[parentActorID];
		phantomFixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(actor);
		body->CreateFixture(&phantomFixtureDef);
	}
}

b2Vec2 VengefulGameEngine::Rigidbody::getPosition()
{
	if (body == nullptr) {
		return b2Vec2(x, y);
	}

	return b2Vec2(body->GetPosition().x, body->GetPosition().y);
}

float VengefulGameEngine::Rigidbody::getRotation()
{
	if (body == nullptr) {
		return rotation;
	}
	float degrees = radiansToDegrees(body->GetAngle());
	return degrees;
}

void VengefulGameEngine::Rigidbody::setBodyType(std::string type)
{
	bodyType = type;

	if (type == "dynamic") {
		bodyTypeBox2D = b2_dynamicBody;
	}
	else if (type == "kinematic") {
		bodyTypeBox2D = b2_kinematicBody;
	}
	else if (type == "static") {
		bodyTypeBox2D = b2_staticBody;
	}

	if (body != nullptr) {
		body->SetType(bodyTypeBox2D);
	}
}

void VengefulGameEngine::Rigidbody::addForce(b2Vec2 forceToAdd)
{
	if (body != nullptr) {
		body->ApplyForceToCenter(forceToAdd, true);
	}
	else {
		startingForce = forceToAdd;
	}
}

void VengefulGameEngine::Rigidbody::setVelocity(b2Vec2 newVelocity)
{
	if (body != nullptr) {
		body->SetLinearVelocity(newVelocity);
	}
	else {
		startingVelocity = newVelocity;
	}
}

void VengefulGameEngine::Rigidbody::setPosition(b2Vec2 newPosition)
{
	if (body == nullptr) {
		x = newPosition.x;
		y = newPosition.y;
	}
	else {
		body->SetTransform(newPosition, body->GetAngle());
	}
}

void VengefulGameEngine::Rigidbody::setRotation(float degreesClockwise)
{
	if (body == nullptr) {
		rotation = degreesClockwise;
	}
	else {
		float radians = degreesToRadians(degreesClockwise);
		body->SetTransform(body->GetPosition(), radians);
	}
}

void VengefulGameEngine::Rigidbody::setAngularVelocity(float degreesClockwise)
{
	if (body != nullptr) {
		body->SetAngularVelocity(degreesToRadians(degreesClockwise));
	}
	else {
		startingAngularVelocity = degreesClockwise;
	}
}

void VengefulGameEngine::Rigidbody::setGravityScale(float newGravityScale)
{
	if (body == nullptr) {
		gravityScale = newGravityScale;
	}
	else {
		body->SetGravityScale(newGravityScale);
	}
}

void VengefulGameEngine::Rigidbody::setUpDirection(b2Vec2 newUpDirection)
{
	newUpDirection.Normalize();

	float newRotation = glm::atan(newUpDirection.y, newUpDirection.x) + (b2_pi * 0.5f);

	if (body == nullptr) {
		rotation = radiansToDegrees(newRotation);
	}
	else {
		body->SetTransform(body->GetPosition(), newRotation);
	}
}

void VengefulGameEngine::Rigidbody::setRightDirection(b2Vec2 newRightDirection)
{
	newRightDirection.Normalize();

	float newRotation = glm::atan(newRightDirection.y, newRightDirection.x);

	if (body == nullptr) {
		rotation = radiansToDegrees(newRotation);
	}
	else {
		body->SetTransform(body->GetPosition(), newRotation);
	}
}

b2Vec2 VengefulGameEngine::Rigidbody::getVelocity()
{
	if (body != nullptr) {
		return body->GetLinearVelocity();
	}
	else {
		return startingVelocity;
	}
}

float VengefulGameEngine::Rigidbody::getAngularVelocity()
{
	if (body != nullptr) {
		return radiansToDegrees(body->GetAngularVelocity());
	}
	else {
		return startingAngularVelocity;
	}
}

float VengefulGameEngine::Rigidbody::getGravityScale()
{
	if (body != nullptr) {
		return body->GetGravityScale();
	}

	return gravityScale;
}

b2Vec2 VengefulGameEngine::Rigidbody::getUpDirection()
{
	float radiansRotation;
	
	if (body != nullptr) {
		radiansRotation = body->GetAngle() - (b2_pi / 2.0f);
	}
	else {
		radiansRotation = degreesToRadians(rotation) - (b2_pi / 2.0f);
	}

	b2Vec2 upVector;

	upVector.x = glm::cos(radiansRotation);

	upVector.y = glm::sin(radiansRotation);
	
	upVector.Normalize();

	return upVector;
}

b2Vec2 VengefulGameEngine::Rigidbody::getRightDirection()
{
	float radiansRotation;
	
	if (body != nullptr) {
		radiansRotation = body->GetAngle();
	}
	else {
		radiansRotation = degreesToRadians(rotation);
	}

	b2Vec2 upVector;

	upVector.x = glm::cos(radiansRotation);

	upVector.y = glm::sin(radiansRotation);

	upVector.Normalize();

	return upVector;
}

void VengefulGameEngine::Rigidbody::onDestroy()
{
	globalPhysicsSystem.world->DestroyBody(body);
	body = nullptr;
}




