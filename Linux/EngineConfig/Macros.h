#ifndef MACROS_H
#define MACROS_H

//#define DEBUG_MODE

#if defined(DEBUG_MODE)

	#define RESOURCES_FOLDER "ResourcePacks/4-1/resources"

	#define STOP_ON_DESIGNATED_FRAME

	#define DESIGNATED_FRAME 30

#else
	#define RESOURCES_FOLDER "resources"
#endif

#endif

// Comment for commit: Boggs