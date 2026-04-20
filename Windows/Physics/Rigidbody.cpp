#include "Rigidbody.h"
#include "Math/Conversions.h"
#include "glm/glm/glm.hpp"

b2Vec2 VengefulGameEngine::Rigidbody::getPosition() const
{
	if (body.b2BodyPtr == nullptr) {
		return position;
	}

	return b2Vec2(body.b2BodyPtr->GetPosition().x, body.b2BodyPtr->GetPosition().y);
}

float VengefulGameEngine::Rigidbody::getRotation() const
{
	if (body.b2BodyPtr == nullptr) {
		return body.rotation;
	}

	float degrees = radiansToDegrees(body.b2BodyPtr->GetAngle());

	return degrees;
}

void VengefulGameEngine::Rigidbody::setBodyType(std::string type)
{
	body.bodyTypeString = type;

	if (type == "dynamic") {
		body.bodyTypeBox2D = b2_dynamicBody;
	}
	else if (type == "kinematic") {
		body.bodyTypeBox2D = b2_kinematicBody;
	}
	else if (type == "static") {
		body.bodyTypeBox2D = b2_staticBody;
	}

	if (body.b2BodyPtr != nullptr) {
		body.b2BodyPtr->SetType(body.bodyTypeBox2D);
	}
}

void VengefulGameEngine::Rigidbody::addForce(b2Vec2 forceToAdd)
{
	if (body.b2BodyPtr == nullptr) {
		body.startingForce = forceToAdd;
		return;
	}

	body.b2BodyPtr->ApplyForceToCenter(forceToAdd, true);

}

void VengefulGameEngine::Rigidbody::setVelocity(b2Vec2 newVelocity)
{
	if (body.b2BodyPtr == nullptr) {
		body.startingVelocity = newVelocity;
		return;
	}

	body.b2BodyPtr->SetLinearVelocity(newVelocity);

}

void VengefulGameEngine::Rigidbody::setPosition(b2Vec2 newPosition)
{
	if (body.b2BodyPtr == nullptr) {
		position = newPosition;
	}
	else {
		body.b2BodyPtr->SetTransform(newPosition, body.b2BodyPtr->GetAngle());
	}
}

float VengefulGameEngine::Rigidbody::getX() const
{
	if (body.b2BodyPtr == nullptr) {
		return position.x;
	}

	b2Vec2 pos = body.b2BodyPtr->GetPosition();
	return pos.x;
}

void VengefulGameEngine::Rigidbody::setX(float x)
{
	if (body.b2BodyPtr == nullptr) {
		position.x = x;
		return;
	}

	b2Vec2 pos = body.b2BodyPtr->GetPosition();
	pos.x = x;
	body.b2BodyPtr->SetTransform(pos, body.b2BodyPtr->GetAngle());
	
}

float VengefulGameEngine::Rigidbody::getY() const
{
	if (body.b2BodyPtr == nullptr) {
		return position.y;
	}

	b2Vec2 pos = body.b2BodyPtr->GetPosition();
	return pos.y;
}

void VengefulGameEngine::Rigidbody::setY(float y)
{
	if (body.b2BodyPtr == nullptr) {
		position.y = y;
		return;
	}

	b2Vec2 pos = body.b2BodyPtr->GetPosition();
	pos.y = y;
	body.b2BodyPtr->SetTransform(pos, body.b2BodyPtr->GetAngle());
}

float VengefulGameEngine::Rigidbody::getAngularFriction() const
{
	if (body.b2BodyPtr == nullptr) {
		return body.angularFriction;
	}

	return body.b2BodyPtr->GetAngularDamping();
}

void VengefulGameEngine::Rigidbody::setAngularFruction(float f)
{
	if (body.b2BodyPtr == nullptr) {
		body.angularFriction = f;
		return;
	}

	body.b2BodyPtr->SetAngularDamping(f);
}

float VengefulGameEngine::Rigidbody::getDensity() const
{
	return body.density;
	// Changing density after a fixture is created is not supported.
}

void VengefulGameEngine::Rigidbody::setDensity(float d)
{
	body.density = d;
	// Changing density after a fixture is created is not supported.
}

bool VengefulGameEngine::Rigidbody::getHasCollider() const
{
	return body.hasCollider;
}

void VengefulGameEngine::Rigidbody::setHasCollider(bool h)
{
	body.hasCollider = true;
}

bool VengefulGameEngine::Rigidbody::getHasTrigger() const
{
	return body.hasTrigger;
}

void VengefulGameEngine::Rigidbody::setHasTrigger(bool h)
{
	body.hasTrigger = h;
}

bool VengefulGameEngine::Rigidbody::getPrecise() const
{
	return body.precise;
}

void VengefulGameEngine::Rigidbody::setPrecise(bool p)
{
	body.precise = true;
}

std::string VengefulGameEngine::Rigidbody::getColliderType() const
{
	return collider.shape;
}

void VengefulGameEngine::Rigidbody::setColliderType(std::string t)
{
	if (t == globalRigidbodyColliderTypes.BOX || t == globalRigidbodyColliderTypes.CIRCLE) {
		collider.shape = t;
	}
}

float VengefulGameEngine::Rigidbody::getColliderWidth() const
{
	return collider.width;
}

void VengefulGameEngine::Rigidbody::setColliderWidth(float w)
{
	collider.width = w;
}

float VengefulGameEngine::Rigidbody::getColliderRadius() const
{
	return collider.radius;
}

void VengefulGameEngine::Rigidbody::setColliderRadius(float r)
{
	collider.radius = r;
}

float VengefulGameEngine::Rigidbody::getColliderHeight() const
{
	return collider.height;
}

void VengefulGameEngine::Rigidbody::setColliderHeight(float h)
{
	collider.height = h;
}

std::string VengefulGameEngine::Rigidbody::getTriggerType() const
{
	return trigger.shape;
}

void VengefulGameEngine::Rigidbody::setTriggerType(std::string t)
{
	if (t == globalRigidbodyColliderTypes.BOX || t == globalRigidbodyColliderTypes.CIRCLE) {
		trigger.shape = t;
	}
}

float VengefulGameEngine::Rigidbody::getTriggerWidth() const
{
	return trigger.width;
}

void VengefulGameEngine::Rigidbody::setTriggerWidth(float w)
{
	trigger.width = w;
}

float VengefulGameEngine::Rigidbody::getTriggerRadius() const
{
	return trigger.radius;
}

void VengefulGameEngine::Rigidbody::setTriggerRadius(float r)
{
	trigger.radius = r;
}

float VengefulGameEngine::Rigidbody::getTriggerHeight() const
{
	return trigger.height;
}

void VengefulGameEngine::Rigidbody::setTriggerHeight(float h)
{
	trigger.height = h;
}

float VengefulGameEngine::Rigidbody::getBounciness() const
{
	return body.bounciness;
}

void VengefulGameEngine::Rigidbody::setBounciness(float b)
{
	body.bounciness = true;
}

float VengefulGameEngine::Rigidbody::getFriction() const
{
	return body.friction;
}

void VengefulGameEngine::Rigidbody::setFriction(float f)
{
	body.friction = f;
}

void VengefulGameEngine::Rigidbody::setRotation(float degreesClockwise)
{
	if (body.b2BodyPtr == nullptr) {
		body.rotation = degreesClockwise;
		return;
	}

	float radians = degreesToRadians(degreesClockwise);

	body.b2BodyPtr->SetTransform(body.b2BodyPtr->GetPosition(), radians);
}

std::string VengefulGameEngine::Rigidbody::getBodyType() const
{
	return body.bodyTypeString;
}

void VengefulGameEngine::Rigidbody::setAngularVelocity(float degreesClockwise)
{
	if (body.b2BodyPtr == nullptr) {
		body.startingAngularVelocity = degreesClockwise;
		return;
	}
	body.b2BodyPtr->SetAngularVelocity(degreesToRadians(degreesClockwise));
}

void VengefulGameEngine::Rigidbody::setGravityScale(float newGravityScale)
{
	if (body.b2BodyPtr == nullptr) {
		body.startingGravityScale = newGravityScale;
		return;
	}
	
	body.b2BodyPtr->SetGravityScale(newGravityScale);
}

void VengefulGameEngine::Rigidbody::setUpDirection(b2Vec2 newUpDirection)
{
	newUpDirection.Normalize();

	float newRotation = glm::atan(newUpDirection.y, newUpDirection.x) + (b2_pi * 0.5f);

	if (body.b2BodyPtr == nullptr) {
		body.rotation = radiansToDegrees(newRotation);
		return;
	}
	
	body.b2BodyPtr->SetTransform(body.b2BodyPtr->GetPosition(), newRotation);
	
}

void VengefulGameEngine::Rigidbody::setRightDirection(b2Vec2 newRightDirection)
{
	newRightDirection.Normalize();

	float newRotation = glm::atan(newRightDirection.y, newRightDirection.x);

	if (body.b2BodyPtr == nullptr) {
		body.rotation = radiansToDegrees(newRotation);
		return;
	}
	
	body.b2BodyPtr->SetTransform(body.b2BodyPtr->GetPosition(), newRotation);
}

b2Vec2 VengefulGameEngine::Rigidbody::getVelocity() const
{
	if (body.b2BodyPtr == nullptr) {
		return body.startingVelocity;
	}
	return body.b2BodyPtr->GetLinearVelocity();
}

float VengefulGameEngine::Rigidbody::getAngularVelocity() const
{
	if (body.b2BodyPtr == nullptr) {
		return body.startingAngularVelocity;
	}

	return radiansToDegrees(body.b2BodyPtr->GetAngularVelocity());

}

float VengefulGameEngine::Rigidbody::getGravityScale() const
{
	if (body.b2BodyPtr == nullptr) {
		return body.startingGravityScale;
	}
	return body.b2BodyPtr->GetGravityScale();
}

b2Vec2 VengefulGameEngine::Rigidbody::getUpDirection() const
{
	float radiansRotation;
	
	if (body.b2BodyPtr == nullptr) {
		radiansRotation = degreesToRadians(body.rotation) - (b2_pi / 2.0f);
	}
	else {
		radiansRotation = body.b2BodyPtr->GetAngle() - (b2_pi / 2.0f);
	}

	b2Vec2 upVector;

	upVector.x = glm::cos(radiansRotation);

	upVector.y = glm::sin(radiansRotation);
	
	upVector.Normalize();

	return upVector;
}

b2Vec2 VengefulGameEngine::Rigidbody::getRightDirection() const
{
	float radiansRotation;
	
	if (body.b2BodyPtr == nullptr) {
		radiansRotation = degreesToRadians(body.rotation);
	}
	else {
		radiansRotation = body.b2BodyPtr->GetAngle();
	}

	b2Vec2 upVector;

	upVector.x = glm::cos(radiansRotation);

	upVector.y = glm::sin(radiansRotation);

	upVector.Normalize();

	return upVector;
}




