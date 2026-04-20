#ifndef GAME_H
#define GAME_H

namespace VengefulGameEngine {
	class Game
	{
	public:
		virtual void initBeforeFirstRender();
		virtual bool isRunning();
		virtual void update();
		virtual void lateUpdate();
		virtual void render();
		virtual void readInput();
		virtual void afterGameEnd();
	private:
		bool running;
	};
}

#endif // GAME_H