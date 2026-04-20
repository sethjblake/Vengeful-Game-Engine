#ifndef LUA_IMAGE_H
#define LUA_IMAGE_H

#include "../Graphics/ImageDrawRequest.h"
#include "../Graphics/GlobalImageDrawRequests.h"
#include "../Graphics/PixelDrawRequest.h"
#include "../Graphics/GlobalPixelDrawRequests.h"

#define SCENE_UNITS 100

using VengefulGameEngine::ImageDrawRequest;
using VengefulGameEngine::PixelDrawRequest;

namespace LuaImage {

	inline void DrawUI(std::string imageName, float x, float y) {
		ImageDrawRequest* drawRequest = new ImageDrawRequest();
		drawRequest->imageName = imageName;
		drawRequest->position.x = x;
		drawRequest->position.y = y;
		drawRequest->color = { 255, 255, 255, 255 };
		drawRequest->scale = { 1.0f, 1.0f };
		drawRequest->pivot = { 0, 0 };
		drawRequest->rotationDegrees = 0;
		drawRequest->sortingOrder = 0;
		VengefulGameEngine::globalImageUIDrawRequests.push_back(drawRequest);
	}

	inline void DrawUIEx(std::string imageName, float x, float y, float r, float g, float b, float a, float sortingOrder) {
		ImageDrawRequest* drawRequest = new ImageDrawRequest();
		drawRequest->imageName = imageName;
		drawRequest->position.x = x;
		drawRequest->position.y = y;
		drawRequest->color.r = static_cast<int>(r);
		drawRequest->color.g = static_cast<int>(g);
		drawRequest->color.b = static_cast<int>(b);
		drawRequest->color.a = static_cast<int>(a);
		drawRequest->sortingOrder = static_cast<int>(sortingOrder);
		drawRequest->scale = { 1.0f, 1.0f };
		drawRequest->pivot = { 0.0f , 0.0f };
		drawRequest->rotationDegrees = 0;
		VengefulGameEngine::globalImageUIDrawRequests.push_back(drawRequest);
	}

	inline void Draw(std::string imageName, float x, float y) {
		ImageDrawRequest* drawRequest = new ImageDrawRequest();
		drawRequest->imageName = imageName;
		drawRequest->position.x = x * SCENE_UNITS;
		drawRequest->position.y = y * SCENE_UNITS;
		drawRequest->color = { 255, 255, 255, 255 };
		drawRequest->scale = { 1.0f, 1.0f };
		drawRequest->pivot = { 0.5f , 0.5f };
		drawRequest->rotationDegrees = 0;
		drawRequest->sortingOrder = 0;
		VengefulGameEngine::globalImageDrawRequests.push_back(drawRequest);
	}

	inline void DrawEx(std::string imageName, float x, float y, float rotationDegrees,
		float scaleX, float scaleY, float pivotX, float pivotY,
		float r, float g, float b, float a, float sortingOrder) {

		ImageDrawRequest* drawRequest = new ImageDrawRequest();
		drawRequest->imageName = imageName;
		drawRequest->position.x = x * SCENE_UNITS;
		drawRequest->position.y = y * SCENE_UNITS;
		drawRequest->color.r = static_cast<int>(r);
		drawRequest->color.g = static_cast<int>(g);
		drawRequest->color.b = static_cast<int>(b);
		drawRequest->color.a = static_cast<int>(a);
		drawRequest->sortingOrder = static_cast<int>(sortingOrder);
		drawRequest->scale = { scaleX, scaleY };
		drawRequest->pivot = { pivotX, pivotY };
		drawRequest->rotationDegrees = static_cast<int>(rotationDegrees);
		VengefulGameEngine::globalImageDrawRequests.push_back(drawRequest);
	}

	inline void DrawPixel(float x, float y, float r, float g, float b, float a) {
		PixelDrawRequest* pixelDrawRequest = new PixelDrawRequest();
		pixelDrawRequest->position.x = static_cast<int>(x);
		pixelDrawRequest->position.y = static_cast<int>(y);
		pixelDrawRequest->color.r = static_cast<int>(r);
		pixelDrawRequest->color.g = static_cast<int>(g);
		pixelDrawRequest->color.b = static_cast<int>(b);
		pixelDrawRequest->color.a = static_cast<int>(a);
		VengefulGameEngine::globalPixelDrawRequests.push_back(pixelDrawRequest);
	}
}

#endif