#ifndef RENDER_CONFIG_REGISTRY_H
#define RENDER_CONFIG_REGISTRY_H

#include "DataBank.h"
#include "../Constants/RenderingConfigConstants.h"

namespace VengefulGameEngine {
	class RenderConfigRegistry {
	public:
		void loadRegistry(DataBank& renderConfigBank);
		RenderingConfigConstants renderingConfigConstants;
	};
}

#endif