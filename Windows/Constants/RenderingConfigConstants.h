#ifndef RENDERING_CONFIG_CONSTANTS_H
#define RENDERING_CONFIG_CONSTANTS_H

#include <string>

namespace VengefulGameEngine {
	class RenderingConfigConstants {
	public:

		const std::string zoom_factor = "zoom_factor";

		const std::string cam_offset_x = "cam_offset_x";

		const std::string cam_offset_y = "cam_offset_y";

		const std::string cam_ease_factor = "cam_ease_factor";
		
		const std::string x_resolution = "x_resolution";
		
		const std::string y_resolution = "y_resolution";
		
		const std::string clear_color_r = "clear_color_r";
		
		const std::string clear_color_g = "clear_color_g";
		
		const std::string clear_color_b = "clear_color_b";
	};
}

#endif
