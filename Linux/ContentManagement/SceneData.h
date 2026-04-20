#ifndef SCENE_DATA_H
#define SCENE_DATA_H

#include <string>
#include <optional>

namespace VengefulGameEngine {
	class SceneData {
	public:
		std::optional<std::string> newSceneName;
		std::string currentScene;
	};
}

#endif