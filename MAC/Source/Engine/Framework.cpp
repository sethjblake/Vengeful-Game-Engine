#include "Framework.h"
#include <iostream>

void VengefulGameEngine::Framework::entryPoint()
{
	initBeforeFirstRender();
	loop();
	afterGameEnd();
}

void VengefulGameEngine::Framework::initBeforeFirstRender()
{
	game.initBeforeFirstRender();
}

void VengefulGameEngine::Framework::loop()
{
	while (game.isRunning()) {
		readInput();
		update();
		lateUpdate();
		render();
	}
}

void VengefulGameEngine::Framework::readInput()
{
	game.readInput();
}

void VengefulGameEngine::Framework::update()
{
	game.update();
}

void VengefulGameEngine::Framework::afterGameEnd()
{
	game.afterGameEnd();
}

void VengefulGameEngine::Framework::render()
{
	game.render();
}

void VengefulGameEngine::Framework::lateUpdate()
{
	game.lateUpdate();
}

