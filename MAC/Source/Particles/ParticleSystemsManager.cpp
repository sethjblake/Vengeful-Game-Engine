#include "ParticleSystemsManager.h"
#include "ParticleSystem.h"

VengefulGameEngine::ParticleSystem* VengefulGameEngine::ParticleSystemsManager::createParticleSystem(int uniqueComponentID, int actorID, std::string key)
{
    actorIDToKeyToIndexMap[actorID][key] = particleSystemsVector.size();
    ParticleSystem* particleSystem = new ParticleSystem();
    particleSystem->uniqueComponentID = uniqueComponentID;
    particleSystemsVector.push_back(particleSystem);
    newParticleSystemsQueue.push_back(uniqueComponentID);
    return particleSystem;
}

void VengefulGameEngine::ParticleSystemsManager::CallOnStart()
{

    for (int i = 0; i < newParticleSystemsQueue.size(); i++) {
        int id = newParticleSystemsQueue[i];
        for (int j = 0; j < particleSystemsVector.size(); j++) {
            if (particleSystemsVector[j]->uniqueComponentID == id) {
                particleSystemsVector[j]->OnStart();
                break;
            }
        }
    }
    newParticleSystemsQueue.clear();
}

void VengefulGameEngine::ParticleSystemsManager::CallUpdate()
{
    for (auto& item : actorIDToKeyToIndexMap) {
        for (auto& stuff : item.second) {
            int index = stuff.second;
            ParticleSystem* particleSystem = particleSystemsVector[index];
            if (particleSystem->enabled) particleSystem->OnUpdate();
        }
    }
}

void VengefulGameEngine::ParticleSystemsManager::DestroyParticleSystem(int id)
{
    for (int i = 0; i < particleSystemsVector.size(); i++) {
        if (particleSystemsVector[i]->uniqueComponentID == id) {
            particleSystemsVector[i]->enabled = false;
            return;
        }
    }
}
