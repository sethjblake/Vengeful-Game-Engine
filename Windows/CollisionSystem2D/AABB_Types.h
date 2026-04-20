#ifndef AABB_TYPES_H
#define AABB_TYPES_H

namespace CollisionSystem2D {

	struct Point2D {
		float x = 0;
		float y = 0;
	};

	struct AABB {
		int uniqueID;
		Point2D min;
		Point2D max;
	};

}

#endif
