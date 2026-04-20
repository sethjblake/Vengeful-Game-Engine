#ifndef GLOBAL_IMAGE_DRAW_REQUESTS_H
#define GLOBAL_IMAGE_DRAW_REQUESTS_H

#include <vector>
#include "../Graphics/ImageDrawRequest.h"

namespace VengefulGameEngine {
	inline std::vector<ImageDrawRequest*> globalImageUIDrawRequests;
	inline std::vector<ImageDrawRequest*> globalImageDrawRequests;
}

#endif