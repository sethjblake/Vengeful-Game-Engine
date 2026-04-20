#ifndef GLOBAL_PIXEL_DRAW_REQUESTS_H
#define GLOBAL_PIXEL_DRAW_REQUESTS_H

#include <vector>
#include "PixelDrawRequest.h"

namespace VengefulGameEngine {
	inline std::vector<PixelDrawRequest*> globalPixelDrawRequests;
}

#endif