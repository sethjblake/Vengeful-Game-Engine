#ifndef LUA_CAMERA_H
#define LUA_CAMERA_H

#include "../Graphics/GlobalCamera.h"

namespace LuaCamera {
	
	void SetPosition(float x, float y) {
		VengefulGameEngine::globalCamera.position.x = x;
		VengefulGameEngine::globalCamera.position.y = y;
	}

	float GetPositionX() {
		return VengefulGameEngine::globalCamera.position.x;
	}

	float GetPositionY() {
		return VengefulGameEngine::globalCamera.position.y;
	}

	void SetZoom(float zoom) {
		VengefulGameEngine::globalCamera.zoomFactor = zoom;
		VengefulGameEngine::globalCamera.zoomFactorInverse = 1 / zoom;
		VengefulGameEngine::newZoomFlag = true;
	}

	float GetZoom() {
		return VengefulGameEngine::globalCamera.zoomFactor;
	}

}

#endif