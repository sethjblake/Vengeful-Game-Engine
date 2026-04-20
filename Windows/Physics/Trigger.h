#ifndef TRIGGER_H
#define TRIGGER_H

namespace VengefulGameEngine {
	struct Trigger {

		std::string shape = globalRigidbodyColliderTypes.BOX;

		float width = 1.0f;

		float height = 1.0f;

		float radius = 0.5f;

	};
}

#endif