#ifndef RIGIDBODY_CONSTANTS_H
#define RIGIDBODY_CONSTANTS_H

#include <string>

namespace VengefulGameEngine {
	struct RigidbodyTypes {

		const std::string DYNAMIC = "dynamic";

		const std::string STATIC = "static";

		const std::string KINEMATIC = "kinematic";

	};

	struct RigidbodyConstants {
		const std::string X = "x";

		const std::string Y = "y";

		const std::string BODY_TYPE = "body_type";

		const std::string GRAVITY_SCALE = "gravity_scale";

		const std::string ROTATION = "rotation";

		const std::string PRECICE = "precise";

		const std::string DENSITY = "density";

		const std::string ANGULAR_FRICTION = "angular_friction";

		const std::string HAS_COLLIDER = "has_collider";

		const std::string HAS_TRIGGER = "has_trigger";

		const std::string RIGIDBODY_ID = "rigidbody_id";

		const std::string COLLIDER_TYPE = "collider_type";

		const std::string WIDTH = "width";

		const std::string HEIGHT = "height";

		const std::string RADIUS = "radius";

		const std::string FRICTION = "friction";

		const std::string BOUNCINESS = "bounciness";

		const std::string PARENT_ACTOR_ID = "parent_actor_id";

		const std::string TRIGGER_WIDTH = "trigger_width";

		const std::string TRIGGER_HEIGHT = "trigger_height";

		const std::string TRIGGER_RADIUS = "trigger_radius";

		const std::string TRIGGER_TYPE = "trigger_type";
	};

	struct RigidbodyColliderTypes {

		const std::string BOX = "box";
		
		const std::string CIRCLE = "circle";
	
	};

	struct RigidbodyFunctionNames {

		const std::string GET_POSITION = "GetPosition";

		const std::string GET_ROTATION = "GetRotation";

		const std::string SET_BODY_TYPE = "SetBodyType";

		const std::string ADD_FORCE = "AddForce";

		const std::string SET_VELOCITY = "SetVelocity";

		const std::string SET_POSITION = "SetPosition";

		const std::string SET_ROTATION = "SetRotation";

		const std::string SET_ANGULAR_VELOCITY = "SetAngularVelocity";

		const std::string SET_GRAVITY_SCALE = "SetGravityScale";

		const std::string GET_GRAVITY_SCALE = "GetGravityScale";

		const std::string SET_UP_DIRECTION = "SetUpDirection";

		const std::string SET_RIGHT_DIRECTION = "SetRightDirection";

		const std::string GET_VELOCITY = "GetVelocity";

		const std::string GET_ANGULAR_VELOCITY = "GetAngularVelocity";

		const std::string GET_UP_DIRECTION = "GetUpDirection";

		const std::string GET_RIGHT_DIRECTION = "GetRightDirection";
	};

}

#endif