#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <vector>
#include "Particle.h"
#include "EECS_404/Helper.h"
#include "Actors/Actor.h"
#include "Graphics/ImagesManager.h"
#include <string>
#include <queue>
#include <optional>
#include <set>

namespace VengefulGameEngine {
	class ParticleSystem {
	public:

		Actor* actor = nullptr;

		int uniqueComponentID;

		std::string key;

		bool enabled = true;

		int localFrameNumber = 0;

		std::string image;

		int sortingOrder = 9999;

		int startColorR = 255;

		int startColorG = 255;

		int startColorB = 255;

		int startColorA = 255;

		void setX(float newX);

		float getX() const;

		float x = 0.0f;

		float y = 0.0f;

		std::vector<Particle> particles;

		std::vector<float> particleXVelocities;

		std::vector<float> particleYVelocities;

		std::vector<float> particleXPositions;

		std::vector<float> particleYPositions;

		std::vector<float> particleAngularVelocities;

		std::vector<float> particleRotations;

		std::vector<int> particleSortingOrders;

		std::vector<int> particleDurationCounters;

		std::vector<uint8_t> particleStartColorR;

		std::vector<uint8_t> particleStartColorG;

		std::vector<uint8_t> particleStartColorB;

		std::vector<uint8_t> particleStartColorA;

		std::vector<uint8_t> particleEndColorR;

		std::vector<uint8_t> particleEndColorG;

		std::vector<uint8_t> particleEndColorB;

		std::vector<uint8_t> particleEndColorA;

		std::vector<uint8_t> particleCurrentColorR;

		std::vector<uint8_t> particleCurrentColorG;

		std::vector<uint8_t> particleCurrentColorB;

		std::vector<uint8_t> particleCurrentColorA;

		std::vector<std::string> particleImageNames;

		std::vector<float> particleLifetimeProgresses;

		std::vector<float> particleInitialScales;

		std::vector<float> particleCurrentScales;

		std::vector<bool> isActive;

		std::vector<bool> lagOneCycle;

		RandomEngine* randomAngleGenerator = nullptr;

		float emitAngleMin = 0.0f;

		float emitAngleMax = 360.0f;

		int randomAngleSeed = 298;

		RandomEngine* randomRadiusGenerator = nullptr;

		float emitRadiusMin = 0.0f;

		float emitRadiusMax = 0.5f;

		int randomRadiusSeed = 404;

		RandomEngine* randomRotationGenerator = nullptr;

		float rotationMin = 0.0f;

		float rotationMax = 0.0f;

		int rotationSeed = 440;

		RandomEngine* randomScaleGenerator = nullptr;

		float startScaleMin = 1.0f;

		float startScaleMax = 1.0f;

		int startScaleSeed = 494;

		RandomEngine* randomSpeedGenerator = nullptr;

		float startSpeedMin = 0.0f;

		float startSpeedMax = 0.0f;

		int startSpeedSeed = 498;

		RandomEngine* randomRotationSpeedGenerator = nullptr;

		float rotationSpeedMin = 0.0f;

		float rotationSpeedMax = 0.0f;

		int rotationSpeedSeed = 305;

		int getFramesBetweenBursts() const;

		void setFramesBetweenBursts(int f);

		int framesBetweenBursts = 1;

		int burstQuantity = 1;

		int getBurstQuantity() const;

		void setBurstQuantity(int f);

		int durationFrames = 300;

		int getDurationFrames() const;

		void setDurationFrames(int d);

		void OnStart();

		void OnUpdate();

		~ParticleSystem();

		std::queue<int> openIndexQueue;

		float gravityScaleX = 0.0f;

		float gravityScaleY = 0.0f;

		float dragFactor = 1.0f;

		float angularDragFactor = 1.0f;

		float endScale = -777.0f;

		int endColorR = -777;
		
		int endColorG = -777;
		
		int endColorB = -777;

		int endColorA = -777;

		int renderOffset = 0;

		int particleCounter = 0;

		std::map<int, int> particleIDToIndexMap;

		std::queue<int> deactivateQueue;

		bool playing = true;

		void Play();

		void Stop();

		void Burst();

		std::set<int> validIndices;

		private:

			void TimeStep();

	};
}

#endif
