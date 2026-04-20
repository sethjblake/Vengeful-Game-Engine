#ifndef VECTOR2_H
#define VECTOR2_H

#include "box2d/box2d.h"

namespace VengefulGameEngine {
	class Vector2 : public b2Vec2 {
	public:
		using b2Vec2::b2Vec2;
	};
}

#endif