#ifndef COLLIDER_H
#define COLLIDER_H

#include <string>
#include <memory>
#include "box2d/box2d.h"
#include "GlobalRigidbodyConstants.h"
#include "Actors/Actor.h"

namespace VengefulGameEngine {
	struct Collider {

		std::string shape = globalRigidbodyColliderTypes.BOX;

		float width = 1.0f;

		float height = 1.0f;

		float radius = 0.5f;

	};
}

#endif