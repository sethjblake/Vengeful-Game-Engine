#ifndef LUA_TEXT_H
#define LUA_TEXT_H

#include <string>
#include "../Graphics/TextRequest.h"
#include "../Graphics/GlobalTextRequests.h"

namespace VengefulGameEngine {
	void Draw(std::string textContent, float x, float y, std::string fontName, int fontSize, float r, float g, float b, float a) {
		TextRequest textRequest;
		textRequest.textContent = textContent;
		textRequest.x = static_cast<int>(x);
		textRequest.y = static_cast<int>(y);
		textRequest.fontName = fontName;
		textRequest.fontSize = fontSize;
		textRequest.r = static_cast<int>(r);
		textRequest.g = static_cast<int>(g);
		textRequest.b = static_cast<int>(b);
		textRequest.a = a;

		globalTextRequests.push(textRequest);
	}
}

#endif