#ifndef TEXT_REQUEST_H
#define TEXT_REQUEST_H

#include <string>

namespace VengefulGameEngine {

	struct TextRequest {

		std::string textContent;

		int x;

		int y;

		std::string fontName;

		int fontSize;

		int r;

		int g;

		int b;

		float a;

	};

}

#endif