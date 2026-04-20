#ifdef _WIN32
#include "../Windows/Engine/Framework.h"
#elif defined(__linux__)
#include "../Linux/Engine/Framework.h"
#elif defined(__APPLE__) && defined(__MACH__)
#include "../MAC/Source/Engine/Framework.h"
#endif

int main(int argc, char* argv[]) {
	VengefulGameEngine::Framework framework;
	framework.entryPoint();
	return 0;
}

// Comment for commit: Piano Woman

