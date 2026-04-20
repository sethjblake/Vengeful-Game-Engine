#include "ParticleSystem.h"
#include "Particle.h"
#include "EECS_404/Helper.h"
#include "glm/glm/glm.hpp"
#include "LuaManager/LuaImage.h"


void VengefulGameEngine::ParticleSystem::setX(float newX)
{
	x = newX;
}

float VengefulGameEngine::ParticleSystem::getX() const
{
	return x;
}

int VengefulGameEngine::ParticleSystem::getFramesBetweenBursts() const
{
	return framesBetweenBursts;
}

void VengefulGameEngine::ParticleSystem::setFramesBetweenBursts(int f)
{
	framesBetweenBursts = (f < 1) ? 1 : f;
}

int VengefulGameEngine::ParticleSystem::getBurstQuantity() const
{
	return burstQuantity;
}

void VengefulGameEngine::ParticleSystem::setBurstQuantity(int f)
{
	burstQuantity = (f < 1) ? 1 : f;
}

int VengefulGameEngine::ParticleSystem::getDurationFrames() const
{
	return durationFrames;
}

void VengefulGameEngine::ParticleSystem::setDurationFrames(int d)
{
	durationFrames = (d < 1) ? 1 : d;
}

void VengefulGameEngine::ParticleSystem::OnStart()
{
	if (randomAngleGenerator != nullptr) return;

	randomAngleGenerator = new RandomEngine(emitAngleMin, emitAngleMax, randomAngleSeed);
	randomRadiusGenerator = new	RandomEngine(emitRadiusMin, emitRadiusMax, randomRadiusSeed);
	randomRotationGenerator = new RandomEngine(rotationMin, rotationMax, rotationSeed);
	randomScaleGenerator = new RandomEngine(startScaleMin, startScaleMax, startScaleSeed);
	randomSpeedGenerator = new RandomEngine(startSpeedMin, startSpeedMax, startSpeedSeed);
	randomRotationSpeedGenerator = new RandomEngine(rotationSpeedMin, rotationSpeedMax, rotationSpeedSeed);

	if (image.empty()) {
		image = "VengefulGameEngineDefaultParticle" + std::to_string(uniqueComponentID);
	}
}

void VengefulGameEngine::ParticleSystem::OnUpdate()
{
	if (randomAngleGenerator == nullptr) return;
	if (!enabled) return;

	if (localFrameNumber % framesBetweenBursts == 0 && playing) {
		Burst();
	}

	TimeStep();

}

VengefulGameEngine::ParticleSystem::~ParticleSystem()
{
	if (randomAngleGenerator != nullptr) {
		delete randomAngleGenerator;
		randomAngleGenerator = nullptr;
	}
	if (randomRadiusGenerator != nullptr) {
		delete randomRadiusGenerator;
		randomRadiusGenerator = nullptr;
	}
	if (randomRotationGenerator != nullptr) {
		delete randomRotationGenerator;
		randomRotationGenerator = nullptr;
	}
	if (randomScaleGenerator != nullptr) {
		delete randomScaleGenerator;
		randomScaleGenerator = nullptr;
	}
	if (randomRotationSpeedGenerator != nullptr) {
		delete randomRotationSpeedGenerator;
		randomRotationSpeedGenerator = nullptr;
	}
	if (randomSpeedGenerator != nullptr) {
		delete randomSpeedGenerator;
		randomRadiusGenerator = nullptr;
	}
}

void VengefulGameEngine::ParticleSystem::Play()
{
	playing = true;
}

void VengefulGameEngine::ParticleSystem::Stop()
{
	playing = false;
}

void VengefulGameEngine::ParticleSystem::Burst()
{
	// Generate random angle steps
	std::vector<float> generatedAngleRadians;
	generatedAngleRadians.resize(burstQuantity);
	for (int i = 0; i < burstQuantity; i++) {
		generatedAngleRadians[i] = glm::radians(randomAngleGenerator->Sample());
	}

	// Calcualte cosines
	std::vector<float> generatedAngleCosines;
	generatedAngleCosines.resize(burstQuantity);
	for (int i = 0; i < burstQuantity; i++) {
		generatedAngleCosines[i] = glm::cos(generatedAngleRadians[i]);;
	}

	// Calculate sines
	std::vector<float> generatedAngleSines;
	generatedAngleSines.resize(burstQuantity);
	for (int i = 0; i < burstQuantity; i++) {
		generatedAngleSines[i] = glm::sin(generatedAngleRadians[i]);;
	}

	// Generate random radii
	std::vector<float> generatedRadii;
	generatedRadii.resize(burstQuantity);
	for (int i = 0; i < burstQuantity; i++) {
		generatedRadii[i] = randomRadiusGenerator->Sample();
	}

	// Generate random speeds
	std::vector<float> generatedSpeeds;
	generatedSpeeds.resize(burstQuantity);
	for (int i = 0; i < burstQuantity; i++) {
		generatedSpeeds[i] = randomSpeedGenerator->Sample();
	}

	std::vector<int> openIndices;

	if (!openIndexQueue.empty()) {

		// Load open indices into vector, enforcing burstQuantity limit on vector
		int counter = 0;
		while (!openIndexQueue.empty() && counter < burstQuantity) {
			int index = openIndexQueue.front();
			openIndices.push_back(openIndexQueue.front());
			validIndices.insert(openIndexQueue.front());
			openIndexQueue.pop();
			counter++;
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleXVelocities[openIndices[i]] = generatedAngleCosines[i] * generatedSpeeds[i];
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleYVelocities[openIndices[i]] = generatedAngleSines[i] * generatedSpeeds[i];
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleXPositions[openIndices[i]] = x + generatedAngleCosines[i] * generatedRadii[i];
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleYPositions[openIndices[i]] = y + generatedAngleSines[i] * generatedRadii[i];
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleAngularVelocities[openIndices[i]] = randomRotationSpeedGenerator->Sample();
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleRotations[openIndices[i]] = randomRotationGenerator->Sample();
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleDurationCounters[openIndices[i]] = 0;
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleStartColorR[openIndices[i]] = startColorR;
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleStartColorG[openIndices[i]] = startColorG;
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleStartColorB[openIndices[i]] = startColorB;
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleStartColorA[openIndices[i]] = startColorA;
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleCurrentColorR[openIndices[i]] = startColorR;
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleCurrentColorG[openIndices[i]] = startColorG;
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleCurrentColorB[openIndices[i]] = startColorB;
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleCurrentColorA[openIndices[i]] = startColorA;
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleLifetimeProgresses[openIndices[i]] = 0.0f;
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleInitialScales[openIndices[i]] = randomScaleGenerator->Sample();
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			particleCurrentScales[openIndices[i]] = particleInitialScales[openIndices[i]];
		}

		for (size_t i = 0; i < openIndices.size(); i++) {
			isActive[openIndices[i]] = true;
		}

	}

	int remainingParticlesToGenerate = burstQuantity - openIndices.size();

	if (remainingParticlesToGenerate == 0) return;

	size_t openIndicesSize = openIndices.size();

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleXVelocities.push_back(generatedAngleCosines[i] * generatedSpeeds[i]);
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleYVelocities.push_back(generatedAngleSines[i] * generatedSpeeds[i]);
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleXPositions.push_back(x + generatedAngleCosines[i] * generatedRadii[i]);
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleYPositions.push_back(y + generatedAngleSines[i] * generatedRadii[i]);
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleAngularVelocities.push_back(randomRotationSpeedGenerator->Sample());
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleRotations.push_back(randomRotationGenerator->Sample());
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleDurationCounters.push_back(0);
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleStartColorR.push_back(startColorR);
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleStartColorG.push_back(startColorG);
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleStartColorB.push_back(startColorB);
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleStartColorA.push_back(startColorA);
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleCurrentColorR.push_back(startColorR);
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleCurrentColorG.push_back(startColorG);
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleCurrentColorB.push_back(startColorB);
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleCurrentColorA.push_back(startColorA);
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleLifetimeProgresses.push_back(0.0f);
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		particleInitialScales.push_back(randomScaleGenerator->Sample());
		particleCurrentScales.push_back(particleInitialScales.back());
	}

	for (size_t i = openIndicesSize; i < burstQuantity; i++) {
		validIndices.insert(isActive.size());
		isActive.push_back(true);
	}
}

void VengefulGameEngine::ParticleSystem::TimeStep()
{
	for (int i : validIndices) {
		particleXVelocities[i] = (particleXVelocities[i] + gravityScaleX) * dragFactor;
	}

	for (int i : validIndices) {
		particleXPositions[i] += particleXVelocities[i];
	}

	for (int i : validIndices) {
		particleYVelocities[i] = (particleYVelocities[i] + gravityScaleY) * dragFactor;
	}

	for (int i : validIndices) {
		particleYPositions[i] += particleYVelocities[i];
	}

	for (int i : validIndices) {
		particleAngularVelocities[i] *= angularDragFactor;
	}

	for (int i : validIndices) {
		particleRotations[i] += particleAngularVelocities[i];
	}

	for (int i : validIndices) {
		particleLifetimeProgresses[i] = static_cast<float>(particleDurationCounters[i]) / static_cast<float>(durationFrames);
	}

	if (endScale != -777.0f) {
		for (int i : validIndices) {
			particleCurrentScales[i] = glm::mix(particleInitialScales[i], endScale, particleLifetimeProgresses[i]);
		}
	}

	if (endColorR != -777) {
		for (int i : validIndices) {
			particleCurrentColorR[i] = glm::mix(particleStartColorR[i], static_cast<uint8_t>(endColorR), particleLifetimeProgresses[i]);
		}
	}

	if (endColorG != -777) {
		for (int i : validIndices) {
			particleCurrentColorG[i] = glm::mix(particleStartColorG[i], static_cast<uint8_t>(endColorG), particleLifetimeProgresses[i]);
		}
	}

	if (endColorB  != -777) {
		for (int i : validIndices) {
			particleCurrentColorB[i] = glm::mix(particleStartColorB[i], static_cast<uint8_t>(endColorB), particleLifetimeProgresses[i]);
		}
	}

	if (endColorA != -777) {
		for (int i : validIndices) {
			particleCurrentColorA[i] = glm::mix(particleStartColorA[i], static_cast<uint8_t>(endColorA), particleLifetimeProgresses[i]);
		}
	}

	for (int i = 0; i < isActive.size(); i++) {
		if (!isActive[i]) {
			continue;
		}

		if (particleDurationCounters[i] >= durationFrames) {
			isActive[i] = false;
			openIndexQueue.push(i);
			validIndices.erase(i);
			particleCounter--;
			continue;
		}

		LuaImage::DrawEx(
			image,
			particleXPositions[i],
			particleYPositions[i],
			particleRotations[i],
			particleCurrentScales[i],
			particleCurrentScales[i],
			0.5f,
			0.5f,
			particleCurrentColorR[i],
			particleCurrentColorG[i],
			particleCurrentColorB[i],
			particleCurrentColorA[i],
			sortingOrder);

	}

	for (int i : validIndices) {
		particleDurationCounters[i]++;
	}

	localFrameNumber++;

}

