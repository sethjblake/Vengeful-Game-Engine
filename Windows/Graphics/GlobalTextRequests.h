#ifndef GLOBAL_TEXT_REQUESTS_H
#define GLOBAL_TEXT_REQUESTS_H

#include <queue>
#include "TextRequest.h"

namespace VengefulGameEngine {
	inline std::queue<TextRequest> globalTextRequests;
}

#endif