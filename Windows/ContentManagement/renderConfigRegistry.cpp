#include "renderConfigRegistry.h"

void VengefulGameEngine::RenderConfigRegistry::loadRegistry(DataBank& renderConfigBank)
{
	renderConfigBank.registerFloat(renderingConfigConstants.zoom_factor, 1.0f);
	renderConfigBank.registerFloat(renderingConfigConstants.cam_offset_x);
	renderConfigBank.registerFloat(renderingConfigConstants.cam_offset_y);
	renderConfigBank.registerFloat(renderingConfigConstants.cam_ease_factor, 1.0f);
	renderConfigBank.registerInt(renderingConfigConstants.x_resolution, 640);
	renderConfigBank.registerInt(renderingConfigConstants.y_resolution, 360);
	renderConfigBank.registerInt(renderingConfigConstants.clear_color_r);
	renderConfigBank.registerInt(renderingConfigConstants.clear_color_g);
	renderConfigBank.registerInt(renderingConfigConstants.clear_color_b);
}
