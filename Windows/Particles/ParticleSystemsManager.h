#ifndef PARTICLE_SYSTEMS_MANAGER_H
#define PARTICLE_SYSTEMS_MANAGER_H

#include <vector>
#include "Particles/Particle.h"
#include "Particles/ParticleSystem.h"
#include <map>
#include "Graphics/ImagesManager.h"

namespace VengefulGameEngine {
	class ParticleSystemsManager {
	public:
		std::vector<int> newParticleSystemsQueue;
		std::map<int, std::map<std::string, int>> actorIDToKeyToIndexMap;
		std::vector<ParticleSystem*> particleSystemsVector;
		ParticleSystem* createParticleSystem(int uniqueComponentID, int actorID, std::string key);
		void CallOnStart();
		void CallUpdate();
		void destroyParticleSystem(int id);
	};
}

#endif