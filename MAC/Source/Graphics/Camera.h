#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm/glm.hpp"

namespace VengefulGameEngine {
	
	class Camera {
	public:

		glm::vec2 position = { 0.0f, 0.0f };

		float zoomFactor = 1.0f;

		float zoomFactorInverse = 1.0f;

	 };
}

#endif