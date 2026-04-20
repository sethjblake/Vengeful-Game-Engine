#ifndef FRAMEWORK_H
#define FRAMEWORK_H

//	#include game header here
#include "Game2D/Game2D.h"

namespace VengefulGameEngine {
	class Framework {
	Game2D game;
	
	public:
		
		//	The first function called by the program, in main.
		void entryPoint();

		//	Prepares for the first game render.
		void initBeforeFirstRender();

		//	Handles the execution of the game loop.
		//	Loop cycles:
		//	Update
		//	Render
		//	Read input
		//  Late update
		void loop();

		//	Tells thegame to render.
		void render();

		//	Tells the game to read input.
		void readInput();

		//	Tells the game to update.
		void update();

		// Updates After Render.
		void lateUpdate();

		//	Runs immediately after game ends.
		void afterGameEnd();
	};
}

#endif // FRAMEWORK_H